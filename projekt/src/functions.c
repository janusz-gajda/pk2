#include <ncurses.h>
#include "cJSON/cJSON.c"

#define REFRESH_INTERVAL 1000000

unsigned int REFRESH_COUNTER = 0;
unsigned long int ITERATION_COUNTER = 0;
unsigned long int POINTER_COUNTER = 1;
unsigned long int COMPARISON_COUNTER = 0;

uint32_t create_hash(char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
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

void print_ncurses(WINDOW *win, int *array, int *positions_y, int *positions_x){
    if(REFRESH_COUNTER < REFRESH_INTERVAL){
        REFRESH_COUNTER++;
    } else{
        print_array_ncurses(win, array, positions_y, positions_x);
        REFRESH_COUNTER = 0;
    }

    ITERATION_COUNTER++; 
}

int sudoku_checker(int *array, int x, int y, int value){
    int err = 0;
    //check, if error in col or in row
    for(int i = 0; i < 9; i++){
        COMPARISON_COUNTER++;
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
            COMPARISON_COUNTER++;
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
        POINTER_COUNTER++;
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
            POINTER_COUNTER++;

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
        POINTER_COUNTER++;

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

void read_array_from_file(FILE *source, int *array){
    int i = 0;
    int p0, p1, p2, p3, p4, p5, p6, p7, p8;
    while(fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8) != EOF){
        *(array + (i*9) + 0) = p0;
        *(array + (i*9) + 1) = p1;
        *(array + (i*9) + 2) = p2;
        *(array + (i*9) + 3) = p3;
        *(array + (i*9) + 4) = p4;
        *(array + (i*9) + 5) = p5;
        *(array + (i*9) + 6) = p6;
        *(array + (i*9) + 7) = p7;
        *(array + (i*9) + 8) = p8; 
        i++;

        if(i > 8)
            break;
    }
}