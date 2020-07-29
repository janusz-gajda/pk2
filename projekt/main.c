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
    int positions_x[9], positions_y[9], grid_before[9][9], grid_after[9][9];
    int *pos_x_p = &positions_x[0];
    int *pos_y_p = &positions_y[0];
    int *grid_before_p = &grid_before[0][0];
    int *grid_after_p = &grid_after[0][0];
    calculate_positions(pos_x_p,pos_y_p);

    if(!argv[1])
        return 0;

    FILE *source = fopen(argv[1], "r");
    read_array_from_file(source, grid_before_p, grid_after_p);

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

    print_array_ncurses(win,grid_after_p,pos_y_p,pos_x_p);
    getch();

    clock_t start = clock();
    sudoku_solver(win,grid_after_p,pos_y_p,pos_x_p,0,0);
    clock_t end = clock();
    
    print_array_ncurses(win,grid_after_p,pos_y_p,pos_x_p);
    getch();

    endwin();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Solving this puzzle took %.3f seconds\n", time_spent);
    FILE *json = fopen("temp.json", "r+");
    if(!json){
        printf("error");
    }
    is_empty(json);
    fseek(json, 0, SEEK_END);
    long fsize = ftell(json);
    fseek(json, 0, SEEK_SET);
    rewind(json);
    char *string_from_file = NULL;
    if(is_empty(json)){
         string_from_file = json_create_missing();
         fsize = strlen(string_from_file);
    } else{
        string_from_file = malloc(fsize + 1);
        fread(string_from_file, 1, fsize, json);
    }
    fclose(json);
    uint32_t hash = create_hash(grid_before_p);
    if(json_compare_hash(string_from_file, fsize+1, hash)){
        printf("Already solved!\n");
    } else{

        char *string_to_file = NULL;
        string_to_file = json_create_object(grid_before_p, grid_after_p, hash, string_from_file, fsize + 1);
        json = fopen("temp.json", "w+");
        fprintf(json, "%s", string_to_file);
        fclose(json);
        free(string_to_file);
    }
    free(string_from_file);    
    printf("Hash: %u\n",create_hash(grid_before_p));

    return 0;
}