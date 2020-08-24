#ifndef GENERIC_H
#define GENERIC_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif  
#define REFRESH_INTERVAL 1000000

extern long REFRESH_COUNTER;

typedef struct arg {   
    struct arg *next;
    struct arg *prev;
    int type;
    int argv_index;
    int enabled;
    
} arg_t;

enum types{
    HISTORY,
    INPUT,
    OUTPUT,
    FORCE,
    PREVIOUS
};

enum enable{
    OFF,
    ON
};

//print help
void help(void);

//save file with puzzle result
int print_to_file(char *, int *);

//load array from CSV file
void read_array_from_file(FILE *, int *, int *);

//called, when file is not found
int file_not_found(FILE *, char *);

//check, if file is empty
int is_empty(FILE *);

//get file size, then rewind
long file_size(FILE *);

//generate x and y coordinates for ncurses print
void calculate_positions(int *, int *);

//create hash from array
uint32_t create_hash(int *);

//print array via ncurses liblary
void print_array_ncurses(WINDOW *, int *, int *, int *);

//call print_array_nucrses every REFRESH_INTERVAL calls
void print_ncurses(WINDOW *, int *, int *, int *);

#endif