/** @file */
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

void help(void);

int print_to_file(char *, int *);

void read_array_from_file(FILE *, int *, int *);

int file_not_found(FILE *, char *);

int is_empty(FILE *);

long file_size(FILE *);

void calculate_positions(int *, int *);

uint32_t create_hash(int *);

void print_array_ncurses(WINDOW *, int *, int *, int *);

void print_ncurses(WINDOW *, int *, int *, int *);

#endif