#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON/cJSON.c"

#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define REFRESH_INTERVAL 1000000

uint32_t REFRESH_COUNTER = 0;

void help(void){
    printf("-h --help - shows help\n");
    printf("-i --input - input CSV formatted puzzle\n");
    printf("-o --output - save solved puzzle into CSV file (only active, when -i is provided)\n");
    printf("-p --previous - shows history of puzzles (disregards all other options)\n");
    printf("-f --force - force solving, even if in hisotry\n");
}

long file_size(FILE *src){
    fseek(src, 0, SEEK_END);
    long fsize = ftell(src);
    fseek(src, 0, SEEK_SET);
    return fsize;
}

int is_empty(FILE *src){
    char ch;
    while((ch = getc(src)) != EOF){
        if(!isspace(ch)){
            rewind(src);
            return 0;
        }
    }
    rewind(src);
    return 1;
}

uint32_t create_hash(int *array){
    uint32_t hash, i;
    for(hash = i = 0; i < 81; ++i)
    {
        hash += *(array + i);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

void print_array_ncurses(WINDOW *win, int *array, int *positions_y, int *positions_x){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(!*(array + (i*9) + j))
                mvwprintw(win, *(positions_y + i), *(positions_x + j), ".");
            else
                mvwprintw(win, *(positions_y + i), *(positions_x + j), "%d", *(array + (i*9) + j));      
        }    
    }
    wrefresh(win);
}
/*
void print_json_ncurses(WINDOW *win, cJSON *array, int*positions_y, int*positions_x){
    int i = 0;
    cJSON *element = NULL;
    cJSON_ArrayForEach(element, array){
        if(!element->valuedouble){
            mvwprintw(win, *(positions_y + (i/9)), *(positions_x + (i%9)), ".");
        } else{
            mvwprintw(win, *(positions_y + (i/9)), *(positions_x + (i%9)), "%d", element->valuedouble);
        }
    }
}
*/
void print_ncurses(WINDOW *win, int *array, int *positions_y, int *positions_x){
    if(REFRESH_COUNTER < REFRESH_INTERVAL){
        REFRESH_COUNTER++;
    } else{
        print_array_ncurses(win, array, positions_y, positions_x);
        REFRESH_COUNTER = 0;
    }

}

int sudoku_checker(int *array, int x, int y, int value){
    int err = 0;
    //check, if error in col or in row
    for(int i = 0; i < 9; i++){
        if(*(array + (y*9) + i) == value && i != x){
            err = 1; 
            break;
        } else if(*(array + (i*9) + x) == value && i != y){ 
            err = 1; 
            break;
        }
    }

    if(err)
        return 0;


    //determine, which 3x3 grid to compare against
    int grid_y, grid_x; //address of first number in grid

    if(y < 3)
        grid_y = 0;
    else if(y < 6)
        grid_y = 3;
    else
        grid_y = 6;

    if(x < 3)
        grid_x = 0;
    else if(x < 6)
        grid_x = 3;
    else
        grid_x = 6;

    //check, if error in 3x3 grid
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(grid_y + i == y || grid_x + j == x){ //we already checked that, also we exclude compare againts it self
                continue;
            }
            if(*(array + (grid_y*9) + (i*9) + grid_x + j) == value){
                err = 1;
                break;
            }
        }
    }

    if(err)
        return 0;
    else
        return 1;

}

int sudoku_validator(int *array){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(*(array + (i*9) + j) == 0)
                continue;
            if(!sudoku_checker(array,j,i,*(array + (i*9) + j)))
                return 0;
            
        }
    }
    return 1;
}

int sudoku_solver(WINDOW *win, int *array, int *positions_y, int *positions_x, int y, int x){
    if(*(array + ( y * 9) + x)){ // if is given as hint, let's go to another one
        if(x == 8 && y == 8) //if is end of array
            return 1;
        else if(x == 8) //if end of row
            return sudoku_solver(win, array, positions_y, positions_x, y+1, 0);
        else
            return sudoku_solver(win, array, positions_y, positions_x, y, x+1);
    } else{ //if is empty 
        for(int z = 1; z < 10; z++){
            print_ncurses(win, array, positions_y, positions_x);

            if(!sudoku_checker(array, x, y, z))
                continue;
            
            *(array + (y*9) + x) = z;

            if(y == 8 && x == 8)
                return 1;
            else if(x == 8)
                if(!sudoku_solver(win,array,positions_y,positions_x,y+1,0))
                    continue;
                else
                    return 1;
            else
                if(!sudoku_solver(win,array,positions_y,positions_x,y,x+1))
                    continue;
                else
                    return 1;    
        }

        *(array + (y*9) + x) = 0;
        print_ncurses(win,array,positions_y,positions_x);

        return 0;
    }
}

void calculate_positions(int *x, int *y){
    int i_offsett = 1;
    int j_offsett = 1;
    for(int i = 0; i < 9; i++){
        *(x + i) = (i*2) + j_offsett;
        *(y + i) = i + i_offsett;

        if(i == 2 || i == 5){
            i_offsett += 1;
            j_offsett += 2;
        }
    }
}

void read_array_from_file(FILE *source, int *array_before, int *array_after){
    int i = 0;
    int p[9];

    for(int j = 0; j < 81; j++){
        *(array_before + j) = *(array_after + j) = 0;
    }

    while(fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5],&p[6],&p[7],&p[8]) != EOF){

        for(int j = 0; j < 9; j++)
            if(p[j] < 0 || p[j] > 9)
                p[j] = 0;
            
        *(array_before + (i*9) + 0) = *(array_after + (i*9) + 0) = p[0];
        *(array_before + (i*9) + 1) = *(array_after + (i*9) + 1) = p[1];
        *(array_before + (i*9) + 2) = *(array_after + (i*9) + 2) = p[2];
        *(array_before + (i*9) + 3) = *(array_after + (i*9) + 3) = p[3];
        *(array_before + (i*9) + 4) = *(array_after + (i*9) + 4) = p[4];
        *(array_before + (i*9) + 5) = *(array_after + (i*9) + 5) = p[5];
        *(array_before + (i*9) + 6) = *(array_after + (i*9) + 6) = p[6];
        *(array_before + (i*9) + 7) = *(array_after + (i*9) + 7) = p[7];
        *(array_before + (i*9) + 8) = *(array_after + (i*9) + 8) = p[8]; 
        i++;

        if(i > 8)
            break;
    }
}

char *json_create_missing(void){
    char *string = NULL;
    cJSON *main = cJSON_CreateObject();
    cJSON *history = cJSON_CreateArray();
    cJSON *hashes = cJSON_CreateArray();
    cJSON_AddItemToObject(main, "history", history);
    cJSON_AddItemToObject(main, "hashes", hashes);
    string = cJSON_Print(main);
    cJSON_Delete(main);
    return string;
}
char *json_create_object(int *array_before, int *array_after, uint32_t hash, char *json_string, long size){
    char *string = NULL;
    cJSON *json_main = cJSON_ParseWithLength(json_string,size);
    cJSON *object = cJSON_CreateObject();
    cJSON *hash_hashes = cJSON_CreateNumber(hash);
    cJSON *hash_history = cJSON_CreateNumber(hash);

    cJSON *before_solving = NULL;
    cJSON *after_solving = NULL;
    cJSON *before_element = NULL;
    cJSON *after_element = NULL;

    cJSON *history = cJSON_GetObjectItemCaseSensitive(json_main, "history");
    cJSON *hashes = cJSON_GetObjectItemCaseSensitive(json_main, "hashes");

    before_solving = cJSON_CreateArray();
    after_solving = cJSON_CreateArray();

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            before_element = cJSON_CreateNumber(*(array_before + (i*9) + j));
            after_element = cJSON_CreateNumber(*(array_after + (i*9) + j));
            cJSON_AddItemToArray(before_solving,before_element);
            cJSON_AddItemToArray(after_solving,after_element);
        }
    }

    cJSON_AddItemToObject(object, "hash", hash_history);
    cJSON_AddItemToObject(object, "before_solving", before_solving);
    cJSON_AddItemToObject(object, "after_solving", after_solving);
    cJSON_AddItemToArray(history, object);
    cJSON_AddItemToArray(hashes, hash_hashes);

    string = cJSON_Print(json_main);
    cJSON_Delete(json_main);
    return string;

}

int json_compare_hash(char *string, long len, uint32_t hash){
    cJSON *main = cJSON_ParseWithLength(string, len);
    cJSON *hashes = cJSON_GetObjectItemCaseSensitive(main, "hashes");
    cJSON *item = NULL;

    cJSON_ArrayForEach(item, hashes){
        if(item->valuedouble == hash){
            cJSON_Delete(main);
            return 1;
        }
    }
    cJSON_Delete(main);
    return 0;
}

int json_find_existing(char *string, long len, uint32_t hash, int *grid_after){
    cJSON *main = cJSON_ParseWithLength(string, len);
    cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
    cJSON *object = NULL;
    cJSON *hash_json = NULL;
    cJSON *grid_from_json = NULL;
    cJSON *element_from_grid = NULL;
    cJSON_ArrayForEach(object, history){
        hash_json = cJSON_GetObjectItemCaseSensitive(object, "hash");
        if(hash_json->valuedouble == hash){
            int i = 0;
            grid_from_json = cJSON_GetObjectItemCaseSensitive(object, "after_solving");
            cJSON_ArrayForEach(element_from_grid, grid_from_json){
                *(grid_after + i) = element_from_grid->valuedouble;
                i++;
            }
            cJSON_Delete(main);
            return 1;
        }
    }
    cJSON_Delete(main);
    return 0;

}

int json_found_problem(void){
    printf("There was a problem with history file! What to do:\n");
    printf("c - continue without history\n");
    printf("n - create new history file\n");
    char ch = getchar();
    if(ch == 'n' || ch == 'N'){
        return 0;
    } else{
        return 2;
    }
}

int json_check_integrity(char *string, long len){
    cJSON *main = cJSON_ParseWithLength(string, len);
    if(main == NULL){
        return json_found_problem();
    } else{
        cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
        cJSON *hashes = cJSON_GetObjectItemCaseSensitive(main, "hashes");
        if(hashes == NULL || history == NULL){
            cJSON_Delete(main);
            return json_found_problem();
        } else{
            cJSON *object = NULL;
            cJSON *grid_before = NULL;
            cJSON *grid_after = NULL;
            cJSON *hash = NULL;
            int i = 0;
            cJSON_ArrayForEach(object, history){
                i++;
                grid_before = cJSON_GetObjectItemCaseSensitive(object, "before_solving");
                grid_after = cJSON_GetObjectItemCaseSensitive(object, "after_solving");
                hash = cJSON_GetObjectItemCaseSensitive(object, "hash");
                if(grid_before == NULL || grid_after == NULL || hash == NULL){
                    cJSON_Delete(main);
                    return json_found_problem();
                } if(!cJSON_IsNumber(hash) || !cJSON_IsArray(grid_before) || !cJSON_IsArray(grid_after) || cJSON_GetArraySize(grid_before) != cJSON_GetArraySize(grid_after) || cJSON_GetArraySize(grid_before) != 81){
                    cJSON_Delete(main);
                    return json_found_problem();
                }
            }
            if(i == 0){
                cJSON_Delete(main);
                return 3;
            }
            cJSON_ArrayForEach(object,hashes){
                if(!cJSON_IsNumber(object)){
                    cJSON_Delete(main);
                    return json_found_problem();
                }
            }
        }
        cJSON_Delete(main);
        return 1;
    }
}

int file_not_found(FILE *src, char *name){
    printf("File %s does not exist, attempting to create it...\n", name);
    src = fopen(name, "w+");
    if(src == NULL){
        printf("Couldn't create file %s, ignoring history...\n", name);
        return 0;
    }
    return 1;
}

int json_history(char *string, long len){
    cJSON *main = cJSON_ParseWithLength(string,len);
    cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
    cJSON *list = history->child;
    cJSON *grid_before_json = NULL;
    cJSON *grid_after_json = NULL;
    cJSON *element = NULL;
    int next_avaliable = 1;
    int previous_avaliable = 0;
    int grid_before[9][9], grid_after[9][9];
    int *grid_before_p = &grid_before[0][0];
    int *grid_after_p = &grid_after[0][0];
    while(1){
        if(list->prev == NULL){
            previous_avaliable = 0;
        } else{
            previous_avaliable = 1;
        }

        if(list->next == NULL){
            next_avaliable = 0;
        } else{
            next_avaliable = 1;
        }
        grid_before_json = cJSON_GetObjectItemCaseSensitive(list, "before_solving");
        grid_after_json = cJSON_GetObjectItemCaseSensitive(list, "after_solving");
        int i = 0;
        cJSON_ArrayForEach(element,grid_before_json){
            if(i < 81)
                *(grid_before_p + i) = element->valuedouble;
            i++;
        }
        i = 0;
        cJSON_ArrayForEach(element,grid_after_json){
            if(i < 81)
                *(grid_after_p + i) = element->valuedouble;
            i++;    
        }
        printf("Puzzle provided:\n\n\t");
        for(i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(grid_before[i][j] == 0){
                    printf(". ");
                } else{
                    printf("%d ", grid_before[i][j]);
                }

                if(j == 2 || j == 5){
                    printf("  ");
                }
            }
            if(i == 2 || i == 5){
                printf("\n");
            } 
            printf("\n\t");
        }
        printf("\n\nPuzzle solved:\n\n\t");
        for(i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                printf("%d ", grid_after[i][j]);
                if(j == 2 || j == 5){
                    printf("  ");
                }
            }
            if(i == 2 || i == 5){
                printf("\n");
            } 
            printf("\n\t");
        }
        if(next_avaliable == 0){
            printf("\n\n e - exit  p - previous\n");
        } else if(previous_avaliable == 0){
            printf("\n\n e - exit  n - next\n");
        } else{
            printf("\n\n e - exit  n - next  p - previous\n");
        }
        i = 0;
        while(!i){
            char ch = getchar();
            if(ch == 'e' || ch == 'E'){
                i = -1;
                break;
            } else if((ch == 'n' || ch == 'N') && next_avaliable){
                i = 1;
                break;
            } else if((ch == 'p' || ch == 'P') && previous_avaliable){
                i = 2;
                break;
            } else if(ch == 'n' || ch == 'N'){
                printf("\nNext is not available\n");
            } else if(ch == 'p' || ch == 'P'){
                printf("\nPrevious is not available\n");
            }
        }
        if(i == 1){
            list = list->next;
            clrscr();
        } else if(i == 2){
            list = list->prev;
            clrscr();
        } else{
            cJSON_Delete(main);
            return 1;
        }

    }
        
    cJSON_Delete(main);
    return 1;
}

int print_to_file(char *str, int *array){
    FILE *src = fopen(str, "w+");
    if(!src)
        return 0;

    for(int i = 0; i < 9; i++)
        fprintf(src, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", *(array + (i*9)), *(array + (i*9) + 1), *(array + (i*9) + 2), *(array + (i*9) + 3), *(array + (i*9) + 4), *(array + (i*9) + 5), *(array + (i*9) + 6), *(array + (i*9) + 7), *(array + (i*9) + 8));
    
    fclose(src);
    return 1;    
}