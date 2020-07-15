/*  checkthis.c  */

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

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
while(l < 100000){
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