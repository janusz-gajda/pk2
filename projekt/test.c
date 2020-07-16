/*  checkthis.c  */
//TODO:
// Set array = 0 when error
// Think, how to act, when error further down the line
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void print_array_ncurses(WINDOW *win,int *array, int *positions_y, int *positions_x){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            mvwprintw (win,*(positions_y + ( i * 8) + j), *(positions_x + ( i * 8) + j), "%d", *(array + ( i * 8) + j));  
        }    
    }
}

int sudoku_solver(WINDOW *win,int *array, int *positions_y, int *positions_x, int y, int x){
    // if is given as hint, let's go to another one
    if(*(array + ( y * 8) + x) != 0){
        //if is end of array
        if(x == 8 && y == 8){
            return 1;
        } // if end of row 
        else if(x == 8){
            return sudoku_solver(win,array,positions_y,positions_x,y+1,0);
        } else{
            return sudoku_solver(win,array,positions_y,positions_x,y,x+1);
        }
    }else{
        //if is empty
        int all_good = 0;
        int z = 1;
        for(z = 1; z < 10; z++){
            *(array + (y*8) + x) = z;
            int err = 0;
            //check, if error in col
            for(int i = 0; i < 9; i++){
                if(i == y){
                    continue;
                }
                else if(*(array + (i*8) + x) == *(array + (y*8) + x)){
                    err = 1; 
                    break;
                }
            }
            if(err == 1){
                continue;
            }
            //check, if error in row
            for(int i = 0; i < 9; i++){
                if(i == x){
                    continue;
                }
                else if(*(array + (y*8) + i) == *(array + (y*8) + x)){
                    err = 1; 
                    break;
                }
            }
             if(err == 1){
                continue;
            }
            //checki, if error in 3x3 grid

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
            else if(y < 6)
                grid_x = 3;
            else
                grid_x = 6;

            int temp_y = grid_y, temp_x = grid_x
            while(temp_y < grid_y+3){
                while(temp_x < grid_x+3){
                    if(*(array + (y*8) + x) == *(array + (grid_y*8) + grid_x)){
                        err = 1;
                        break;
                    }
                    temp_x++;
                }
                temp_x = grid_x;
                temp_y++;
            }
            if(err == 1){
                continue;
            }
            all_good = 1;         

        }
        if(all_good == 0){
            return 0;
        }else{
            int temp;
            if(x == 8 && y == 8){
                return 1;
            } // if end of row 
            else if(x == 8){
                temp = sudoku_solver(win,array,positions_y,positions_x,y+1,0);
            } else{
                temp = sudoku_solver(win,array,positions_y,positions_x,y,x+1);
            }
            if(temp == 1){
                return 1;
            }else
        }
    }
}
/*int calculate_x(int &x[9][9], int &y){
int i_offsett = 1;
int j_offsett = 1;
int i_counter = 0;
int j_counter = 0;
for(int i = 0; i < 9; i++){
        i_counter++;
        for(int j = 0; j < 9; j++){
            j_counter++;
            x[i][j] = (j*2)+j_offsett;
            y[i][j] = i+i_offsett;
            if(j_counter == 3 || j_counter == 6)
                j_offsett +=2;
        }
        j_offsett = 1;
        j_counter = 0;
        if(i_counter == 3 || i_counter == 6)
            i_offsett++;
    }
}*/

int main(){
clock_t begin = clock();
int positions_x[9][9], positions_y[9][9], grid[9][9];
int i_offsett = 1;
int j_offsett = 1;
int i_counter = 0;
int j_counter = 0;
//Generating positions to print
for(int i = 0; i < 9; i++){
        i_counter++;
        for(int j = 0; j < 9; j++){
            j_counter++;
            positions_x[i][j] = (j*2)+j_offsett;
            positions_y[i][j] = i+i_offsett;
            if(j_counter == 3 || j_counter == 6)
                j_offsett +=2;
        }
        j_offsett = 1;
        j_counter = 0;
        if(i_counter == 3 || i_counter == 6)
            i_offsett++;
    }
    i_offsett = 1;
    i_counter = 0;
int seed;
time_t tt;
int c = 0;
seed = time(&tt);
srand(seed);
//Starting ncurses screen
initscr();
crmode();
cbreak();
int startx, starty, width, height;
height = 13;
width = 23;
starty = (LINES - height) / 2;
startx = (COLS - width) / 2;
WINDOW *win = newwin(height, width, starty, startx);
refresh();
//box(win,0,0);
wrefresh(win);


curs_set (0);
int l = 0;
while(l < 10000){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            c = rand() % 10;
            int y = i+i_offsett;
            int x = (j*2)+j_offsett;
            mvwprintw (win,positions_y[i][j], positions_x[i][j], "%d", c);
            
        }    
    }
    wrefresh (win);
    i_offsett = 1;
    i_counter = 0;
    l++;
    //int k = getch();
}    
endwin();
clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
printf("1000 iterations took %f seconds\n", time_spent);
return 0;

}