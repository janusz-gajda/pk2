/** @file */
#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "generic.h"
#include "param.h"


char *json_create_missing(void);

char *json_create_object(int *, int *, uint32_t, char *, long);

int json_compare_hash(char *, long, uint32_t);

int json_find_existing(char *, long, uint32_t, int *);

int json_found_problem(void);

int json_check_integrity(char *, long);

int json_history(char *, long);

char *start_history(arg_t *, long *);

void start_previous(arg_t *, char *, long);

#endif