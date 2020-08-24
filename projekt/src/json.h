#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "generic.h"
#include "param.h"

//create empty JSON objects
char *json_create_missing(void);

//add entry for puzlle into JSON
char *json_create_object(int *, int *, uint32_t, char *, long);

//check, if entry already exists
int json_compare_hash(char *, long, uint32_t);

//get object form JSON
int json_find_existing(char *, long, uint32_t, int *);

//called, when problem occured with JSON file
int json_found_problem(void);

//check, if JSON file is valid
int json_check_integrity(char *, long);

//iterate through JSON file
int json_history(char *, long);

//initialize JSON
char *start_history(arg_t *, long *);

//previous function
void start_previous(arg_t *, char *, long);


#endif