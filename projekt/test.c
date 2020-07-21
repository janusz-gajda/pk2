//TODO:
// Set array = 0 when error DONE, we set it when valid
// Think, how to act, when error further down the line, DONE
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void print_array_ncurses(WINDOW *win,int *array, int *positions_y, int *positions_x){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(*(array + (i*9) + j) == 0)
                mvwprintw (win,*(positions_y + i), *(positions_x + j), "-");
            else
                mvwprintw (win,*(positions_y + i), *(positions_x + j), "%d", *(array + (i*9) + j));      
        }    
    }
    wrefresh(win);
}

int sudoku_checker(int *array,int x, int y, int value){
            int err = 0;
            //check, if error in col
            for(int i = 0; i < 9; i++){
                if(i == y){
                    continue;
                }
                else if(*(array + (i*9) + x) == value){
                    err = 1; 
                    break;
                }
            }
            if(err == 1){
                return 0;
            }
            //check, if error in row
            for(int i = 0; i < 9; i++){
                if(i == x){
                    continue;
                }
                else if(*(array + (y*9) + i) == value){
                    err = 1; 
                    break;
                }
            }
             if(err == 1){
                return 0;
            }
            //check, if error in 3x3 grid

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

            int temp_y = grid_y, temp_x = grid_x;
            while(temp_y < grid_y+3){
                while(temp_x < grid_x+3){
                    if(temp_y == y || temp_x == x){
                        //we already checked that, also we exclude comare againts it self
                        continue;
                    }
                    if(value == *(array + (grid_y*9) + grid_x)){
                        err = 1;
                        break;
                    }
                    temp_x++;
                }
                temp_x = grid_x;
                temp_y++;
            }
            if(err == 1){
                return 0;
            }
            return 1;      

        }

int sudoku_solver(int *array, int *positions_y, int *positions_x, int y, int x){ //WINDOW *win,
    // if is given as hint, let's go to another one
    printf("x = %d y = %d\n",x,y);
    if(*(array + ( y * 9) + x) != 0){
        //if is end of array
        if(x == 8 && y == 8){
            return 1;
        } // if end of row 
        else if(x == 8){
            return sudoku_solver(array,positions_y,positions_x,y+1,0);
        } else{
            return sudoku_solver(array,positions_y,positions_x,y,x+1);
        }
    }else{
        //if is empty
        int z = 1;
        int valid = 0;
        int next;
        for(z = 1; z < 10; z++){
            //mvwprintw(win,*(positions_y + y), *(positions_x + x), "%d", z);
            //wrefresh(win);
            printf("z = %d\n", z);
            valid = sudoku_checker(array, x, y, z);
            if(valid == 0)
                continue;

            *(array + (y*9) + x) = z;
            if(y == 8 && x == 8){
                return 1;
            }else if(x == 8){
                next = sudoku_solver(array,positions_y,positions_x,y+1,0);
            } else{
                next = sudoku_solver(array,positions_y,positions_x,y,x+1);
            }
            if(next == 0){
                continue;
            }else{
                return 1;
            }
        }
        *(array + (y*9) + x) = 0;
        //mvwprintw(win,*(positions_y + y), *(positions_x + x), "%c", '-');
        //wrefresh(win);
        return 0;

        
    }
}
void calculate_positions(int *x, int *y){
int i_offsett = 1;
int j_offsett = 1;
for(int i = 0; i < 9; i++){
    *(x + i) = (i*2)+j_offsett;
    *(y + i) = i+i_offsett;
    if(i == 2 || i == 5){
        i_offsett += 1;
        j_offsett += 2;
    }
}
}

int main(int arg, char *argv[]){
clock_t begin = clock();
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
while((fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8)) != EOF){
    grid[i][0] = p0;
    grid[i][1] = p1;
    grid[i][2] = p2;
    grid[i][3] = p3;
    grid[i][4] = p4;
    grid[i][5] = p5;
    grid[i][6] = p6;
    grid[i][7] = p7;
    grid[i][8] = p8;
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n",grid[i][0],grid[i][1],grid[i][2],grid[i][3],grid[i][4],grid[i][5],grid[i][6],grid[i][7],grid[i][8]);
    i++;
    if(i > 8)
        break;
}


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
//WINDOW *win = newwin(height, width, starty, startx);
sudoku_solver(grid_p,pos_y_p,pos_x_p,0,0);
//refresh();
//box(win,0,0);
/*wrefresh(win);


curs_set (0);
print_array_ncurses(win,grid_p,pos_y_p,pos_x_p);

print_array_ncurses(win,grid_p,pos_y_p,pos_x_p);*/
getch();
/*int l = 0;
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
}  */  
//endwin();
/*clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
printf("1000 iterations took %f seconds\n", time_spent);*/
return 0;

}