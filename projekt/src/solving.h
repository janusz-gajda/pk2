#ifndef SOLVING_H
#define SOLVING_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "generic.h"
#include "param.h"
#include "json.h"

//check, if provided value can be inserted
int sudoku_checker(int *, int , int , int);

//check, if there are no contradictions in input file
int sudoku_validator(int *);

//main solving function
int sudoku_solver(WINDOW *, int *, int *, int *, int, int);

//start solving
int start_solver(arg_t *, char *, long, char *[]);

#endif