/** @file */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include "generic.h"

long REFRESH_COUNTER = 0;

/**Print help*/
void help(void){
    printf("-h --help - shows help\n");
    printf("-i --input - input CSV formatted puzzle\n");
    printf("-o --output - save solved puzzle into CSV file (only active, when -i is provided)\n");
    printf("-p --previous - shows history of puzzles (disregards all other options)\n");
    printf("-f --force - force solving, even if in hisotry\n");
}

/**Print array to file
 * @param str file name
 * @param array array of values
*/
int print_to_file(char *str, int *array){
    FILE *src = fopen(str, "w+");
    if(!src)
        return 0;

    for(int i = 0; i < 9; i++)
        fprintf(src, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", *(array + (i*9)), *(array + (i*9) + 1), *(array + (i*9) + 2), *(array + (i*9) + 3), *(array + (i*9) + 4), *(array + (i*9) + 5), *(array + (i*9) + 6), *(array + (i*9) + 7), *(array + (i*9) + 8));
    
    fclose(src);
    return 1;    
}
/**Load array from CSV file
 * @param source file
 * @param array_before array before solving
 * @param array_after array after solving
*/
void read_array_from_file(FILE *source, int *array_before, int *array_after){
    int i = 0;
    int p[9];

    //set all values to zero
    for(int j = 0; j < 81; j++){
        *(array_before + j) = *(array_after + j) = 0;
    }

    while(fscanf(source, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5],&p[6],&p[7],&p[8]) != EOF){

        for(int j = 0; j < 9; j++){
            if(p[j] < 0 || p[j] > 9)
                p[j] = 0;
            *(array_before + (i*9) + j) = *(array_after + (i*9) + j) = p[j];    
        }    
        i++;

        if(i > 8)
            break;
    }
}

/**Called, when file is not found
 * @param src file
 * @param name file name
*/
int file_not_found(FILE *src, char *name){
    printf("File %s does not exist, attempting to create it...\n", name);
    src = fopen(name, "w+");
    if(src == NULL){
        printf("Couldn't create file %s, ignoring history...\n", name);
        return 0;
    }
    return 1;
}
/**Check, if file is empty
 * @param src file
*/
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

/**Get file size, then rewind
 * @param src file
*/
long file_size(FILE *src){
    fseek(src, 0, SEEK_END);
    long fsize = ftell(src);
    fseek(src, 0, SEEK_SET);
    return fsize;
}

/**Generate x and y coordinates for ncurses print
 * @param x array for x values
 * @param y array for y values
*/
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

/**Create hash from array
 * @param array array
*/
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

/**Print array via ncurses liblary
 * @param win ncurses window
 * @param array values to print
 * @param positions_y values for printing in y axis
 * @param positions_x values for printing in x axis
*/
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

/**Call print_array_nucrses every REFRESH_INTERVAL calls
 * @param win ncurses window
 * @param array values to print
 * @param positions_y values for printing in y axis
 * @param positions_x values for printing in x axis
*/
void print_ncurses(WINDOW *win, int *array, int *positions_y, int *positions_x){
    if(REFRESH_COUNTER < REFRESH_INTERVAL){
        REFRESH_COUNTER++;
    } else{
        print_array_ncurses(win, array, positions_y, positions_x);
        REFRESH_COUNTER = 0;
    }

}
