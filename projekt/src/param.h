/** @file */
#ifndef PARAM_H
#define PARAM_H

#include <stdlib.h>
#include "generic.h"

#define stop_program(head, string) param_destroy_struct(head); if(string != NULL){free(string); string = NULL;}; return 0;

void add_to_end(arg_t *);

arg_t *param_create_struct(void);

void param_disable_history(arg_t *);

void param_destroy_struct(arg_t *);

int param_status_input(arg_t *);

int param_status_output(arg_t *);

int param_status_history(arg_t *);

int param_status_force(arg_t *);

int param_status_previous(arg_t *);

void param_create_input(arg_t *, int);

void param_create_output(arg_t *, int);

void param_create_force(arg_t *);

void param_create_previous(arg_t *);

#endif