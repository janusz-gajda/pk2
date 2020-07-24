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
#include "src/functions.c"

int main(int arg, char *argv[]){
    int positions_x[9], positions_y[9], grid[9][9];
    int *pos_x_p = &positions_x[0];
    int *pos_y_p = &positions_y[0];
    int *grid_p = &grid[0][0];
    calculate_positions(pos_x_p,pos_y_p);

    if(!argv[1])
        return 0;

    FILE *source = fopen(argv[1], "r");
    read_array_from_file(source,grid_p);

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
    printf("Iterations:    %lu\n", ITERATION_COUNTER);
    printf("Comparisons:   %lu\n", COMPARISON_COUNTER);
    printf("Pointer moves: %lu\n\n", POINTER_COUNTER);
    printf("Iterations per second:     %.2f\n", (double)(ITERATION_COUNTER / time_spent));
    printf("Comparisons per second:    %.2f\n", (double)(COMPARISON_COUNTER / time_spent));
    printf("Pointers moves per second: %.2f\n", (double)(POINTER_COUNTER / time_spent));

    return 0;
}