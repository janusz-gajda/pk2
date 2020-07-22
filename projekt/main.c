//CLI arguments
//-h --help - shows help
//-i --input - input CSV formated puzzle
//-s --setup - launch into CLI menu
//-o --output - save solved puzzle into CSV file
//-p --previous - shows history of puzzles
//-l [id] --launch [id] - launch privious puzzle
//[none] - shows help

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

unsigned int REFRESH_COUNTER = 0;
unsigned long int ITERATION_COUNTER = 0;
unsigned long int POINTER_COUNTER = 1;
unsigned long int COMPARISON_COUNTER = 0;

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
    if(REFRESH_COUNTER < 500000){
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

int main(int arg, char *argv[]){
    int positions_x[9], positions_y[9], grid[9][9];
    int *pos_x_p = &positions_x[0];
    int *pos_y_p = &positions_y[0];
    int *grid_p = &grid[0][0];
    calculate_positions(pos_x_p,pos_y_p);

    if(!argv[1])
        return 0;

    FILE *source = fopen(argv[1], "r");
    int p0, p1, p2, p3, p4, p5, p6, p7, p8;
    int i = 0;
    while(fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8) != EOF){
        grid[i][0] = p0;
        grid[i][1] = p1;
        grid[i][2] = p2;
        grid[i][3] = p3;
        grid[i][4] = p4;
        grid[i][5] = p5;
        grid[i][6] = p6;
        grid[i][7] = p7;
        grid[i][8] = p8; 
        i++;

        if(i > 8)
            break;
    }

    //Starting ncurses screen
    initscr();
    crmode();
    cbreak();

    int start_x, start_y, width, height;
    height = 13;
    width = 23;
    start_y = (LINES - height) / 2;
    start_x = (COLS - width) / 2;

    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();
    wrefresh(win);
    curs_set (0);

    print_array_ncurses(win,grid_p,pos_y_p,pos_x_p);
    getch();

    clock_t start = clock();
    sudoku_solver(win,grid_p,pos_y_p,pos_x_p,0,0);
    clock_t end = clock();
    
    print_array_ncurses(win,grid_p,pos_y_p,pos_x_p);
    getch();

    endwin();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Solving this puzzle took %.3f seconds\n", time_spent);
    printf("Iterations: %lu\n", ITERATION_COUNTER);
    printf("Comparisons: %lu\n", COMPARISON_COUNTER);
    printf("Pointer moves: %lu\n", POINTER_COUNTER);
    printf("Iterations per second: %.2f\n", (double)(ITERATION_COUNTER / time_spent));
    printf("Comparisons per second: %.2f\n", (double)(COMPARISON_COUNTER / time_spent));
    printf("Pointers moves per second: %.2f\n", (double)(POINTER_COUNTER / time_spent));

    return 0;
}