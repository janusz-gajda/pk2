/** @file */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "generic.h"
#include "param.h"
#include "json.h"
#include "solving.h"

/**Check, if provided value can be inserted
 * @param array array of values
 * @param x x position
 * @param y y position
 * @param value value to check
*/
int sudoku_checker(int *array, int x, int y, int value){
    //check, if error in column or in row
    for(int i = 0; i < 9; i++){
        if(*(array + (y*9) + i) == value && i != x){
            return 0;
        } else if(*(array + (i*9) + x) == value && i != y){ 
            return 0;
        }
    }


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
                return 0;
            }
        }
    }

   return 1;

}

/**Check, if there are no contradictions in input file
 * @param array array of values
*/
int sudoku_validator(int *array){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(*(array + (i*9) + j) == 0)
                continue;
            if(!sudoku_checker(array, j, i, *(array + (i*9) + j)))
                return 0;
            
        }
    }
    return 1;
}

/**Main solving function
 * @param win ncurses window
 * @param arrray array of values
 * @param positions_y coordinates for printing in y axis
 * @param positions_x coordinates for printing in x axis
 * @param y position y
 * @param x position x
*/
int sudoku_solver(WINDOW *win, int *array, int *positions_y, int *positions_x, int y, int x){
    if(*(array + (y*9) + x)){ // if is given as hint, let's go to another one
        if(x == 8 && y == 8) //if is end of array
            return 1;
        else if(x == 8) //if end of row
            return sudoku_solver(win, array, positions_y, positions_x, y+1, 0);
        else
            return sudoku_solver(win, array, positions_y, positions_x, y, x+1);
    } else{ //if is empty 
        for(int z = 1; z < 10; z++){
            print_ncurses(win, array, positions_y, positions_x);

            if(!sudoku_checker(array, x, y, z)) //if z cannot be inserted, continue
                continue;
            
            //if z can be inserted, check next entry in array
            *(array + (y*9) + x) = z;

            if(y == 8 && x == 8)//if end of array
                return 1;
            else if(x == 8)//if end of row
                if(!sudoku_solver(win, array, positions_y, positions_x, y+1, 0))//if there is an error further down the line, increase z
                    continue;
                else
                    return 1;//if everything is in order, return 1
            else 
                if(!sudoku_solver(win, array, positions_y, positions_x, y, x+1))//if there is an error further down the line, increase z
                    continue;
                else
                    return 1;//if everything is in order, return 1    
        }
        //if there are no possible combinations return 0
        *(array + (y*9) + x) = 0;
        print_ncurses(win,array,positions_y,positions_x);

        return 0;
    }
}

/**Start solving
 * @param head head of struct
 * @param string contents of JSON file
 * @param fsize size of JSON file
 * @param argv[] argv array
*/
int start_solver(arg_t *head, char *string, long fsize, char *argv[]){
    int positions_x[9], positions_y[9], grid_before[9][9], grid_after[9][9];
    int *pos_x_p = &positions_x[0];
    int *pos_y_p = &positions_y[0];
    int *grid_before_p = &grid_before[0][0];
    int *grid_after_p = &grid_after[0][0];

    int input_index = param_status_input(head);
    int check = 0;
    calculate_positions(pos_x_p,pos_y_p);
    FILE *source = fopen(argv[input_index], "r");
    if(source == NULL){
        printf("Cannot open %s\n",argv[input_index]);
        return 0;
    }

    read_array_from_file(source, grid_before_p, grid_after_p);
    fclose(source);

    if(!sudoku_validator(grid_before_p)){
        printf("Provided puzzle is unsolvable!\n");
        return 0;
    }

    uint32_t hash = create_hash(grid_before_p);

    if(param_status_history(head)){
        if(json_compare_hash(string, fsize+1, hash)){
            if(!param_status_force(head)){
                check = json_find_existing(string, fsize+1, hash, grid_after_p);
                if(check){
                    printf("This puzzle has already been solved, printing solution\n");
                }
            }   
        }
    }
    //start nucurses window
    initscr();
    crmode();
    cbreak();

    int start_x, start_y, width, height;
    clock_t start, end;
    height = 13;
    width = 23;
    start_y = (LINES - height) / 2;
    start_x = (COLS - width) / 2;

    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();
    wrefresh(win);
    curs_set (0);
            
    print_array_ncurses(win,grid_after_p,pos_y_p,pos_x_p);

    if(!check){
        start = clock();
        int status =  sudoku_solver(win,grid_after_p,pos_y_p,pos_x_p,0,0);
        end = clock();
        print_array_ncurses(win,grid_after_p,pos_y_p,pos_x_p);
        if(!status){
            endwin();
            printf("Provided puzzle is unsolvable!\n");
            return 0;
        }
    }

    print_array_ncurses(win,grid_after_p,pos_y_p,pos_x_p);
    getch();
    endwin();

    if(!check){
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Solving this puzzle took %.3f seconds\n", time_spent);
        if(param_status_history && (!param_status_force(head) || (param_status_force(head) && !check))){
            //add object to JSON file
            char *string_to_file = NULL;
            string_to_file = json_create_object(grid_before_p, grid_after_p, hash, string, fsize + 1);
            FILE *json = fopen("history.json", "w+");
            fprintf(json, "%s", string_to_file);
            fclose(json);
            free(string_to_file);
        }
    }
              
    if(param_status_output(head)){
        //if output is activated, save to file
        print_to_file(argv[param_status_output(head)], grid_after_p);
    }

    return 0;
}