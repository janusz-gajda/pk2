#ifndef PARAM_H
#define PARAM_H

#include <stdlib.h>
#include "generic.h"

#define stop_program(head, string) param_destroy_struct(head); if(string != NULL){free(string); string = NULL;}; return 0;

//allocate memory at the end of list
void add_to_end(arg_t *);

//create first element of list
arg_t *param_create_struct(void);

//disable history file
void param_disable_history(arg_t *);

//delete list
void param_destroy_struct(arg_t *);

//check, if input is activated
int param_status_input(arg_t *);

//check, if output is activated
int param_status_output(arg_t *);

//check, if history is activated
int param_status_history(arg_t *);

//check, if force is activated
int param_status_force(arg_t *);

//check, if previous is activated
int param_status_previous(arg_t *);

//create entry for input flag
void param_create_input(arg_t *, int);

//create entry for output flag
void param_create_output(arg_t *, int);

//create entry for force flag
void param_create_force(arg_t *);

//create entry for previous flag
void param_create_previous(arg_t *);

#endif