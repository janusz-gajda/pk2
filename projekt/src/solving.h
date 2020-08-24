/** @file */
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

int sudoku_checker(int *, int , int , int);

int sudoku_validator(int *);

int sudoku_solver(WINDOW *, int *, int *, int *, int, int);

int start_solver(arg_t *, char *, long, char *[]);

#endif