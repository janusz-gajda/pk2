#include <ncurses.h>
#include <stdlib.h>
#include "cJSON/cJSON.c"

#define REFRESH_INTERVAL 1000000

uint32_t REFRESH_COUNTER = 0;

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
    int p0, p1, p2, p3, p4, p5, p6, p7, p8;
    while(fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8) != EOF){
        *(array_before + (i*9) + 0) = *(array_after + (i*9) + 0) = p0;
        *(array_before + (i*9) + 1) = *(array_after + (i*9) + 1) = p1;
        *(array_before + (i*9) + 2) = *(array_after + (i*9) + 2) = p2;
        *(array_before + (i*9) + 3) = *(array_after + (i*9) + 3) = p3;
        *(array_before + (i*9) + 4) = *(array_after + (i*9) + 4) = p4;
        *(array_before + (i*9) + 5) = *(array_after + (i*9) + 5) = p5;
        *(array_before + (i*9) + 6) = *(array_after + (i*9) + 6) = p6;
        *(array_before + (i*9) + 7) = *(array_after + (i*9) + 7) = p7;
        *(array_before + (i*9) + 8) = *(array_after + (i*9) + 8) = p8; 
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

int json_check_integrity(char *string, long len){
    cJSON *main = cJSON_ParseWithLength(string, len);
    if(main == NULL){
        printf("There was a problem with history file! What to do:\n");
        printf("c - continue without history\n");
        printf("n - create new history file\n");
        char ch;
        ch = getchar();
        printf("%c",ch);
        if(ch == 156 || ch == 116){
            return 0;
        } else{
            return 2;
        }
    } else{
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

void json_history(char *string){
    cJSON *main = cJSON_Parse(string);

        
    free(main);
}

int print_to_file(char *str, int *array){
    FILE *src = fopen(str, "w+");
    if(!src)
        return 0;

    for(int i = 0; i < 9; i++){
        fprintf(src, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", *(array + (i*9)), *(array + (i*9) + 1), *(array + (i*9) + 2), *(array + (i*9) + 3), *(array + (i*9) + 4), *(array + (i*9) + 5), *(array + (i*9) + 6), *(array + (i*9) + 7), *(array + (i*9) + 8));
    }
    fclose(src);
    return 1;    
}