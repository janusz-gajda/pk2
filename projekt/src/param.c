#include <stdlib.h>
#include "generic.h"
#include "param.h"

void add_to_end(arg_t *head){
    while(head->next != NULL){
        head = head->next;
    }
    head->next = (arg_t *)malloc(sizeof(arg_t));
    head->next->prev = head;
    head = head->next;
    head->next = NULL;
}

arg_t *param_create_struct(void){
    arg_t *head = (arg_t *)malloc(sizeof(arg_t));
    head->next = NULL;
    head->prev = NULL;
    head->type = HISTORY;
    head->argv_index = 0;
    head->enabled = ON;
    return head;
}

void param_disable_history(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == HISTORY){
            head_p->enabled = OFF;
        }
        head_p = head_p->next;
    }
}

void param_destroy_struct(arg_t *head){
    while(head != NULL){
        arg_t *next = head->next;
        free(head);
        head = next;
    }
}

int param_status_input(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == INPUT && head_p->enabled == ON){
            return head_p->argv_index;
        }
        head_p = head_p->next;
    }
    return 0;
}

int param_status_output(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == OUTPUT && head_p->enabled == ON){
            return head_p->argv_index;
        }
        head_p = head_p->next;
    }
    return 0;
}

int param_status_history(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == HISTORY && head_p->enabled == ON){
            return 1;
        }
        head_p = head_p->next;
    }
    return 0;
}
int param_status_force(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == FORCE && head_p->enabled == ON){
            return 1;
        }
        head_p = head_p->next;
    }
    return 0;
}

int param_status_previous(arg_t *head){
    arg_t *head_p = head;
    while(head_p != NULL){
        if(head_p->type == PREVIOUS && head_p->enabled == ON){
            return 1;
        }
        head_p = head_p->next;
    }
    return 0;
}

void param_create_input(arg_t *head, int argv_index){
    if(!param_status_input(head)){
        arg_t *head_p = head;
        add_to_end(head_p);
        while(head_p->next != NULL){
            head_p = head_p->next;
        }
        head_p->type = INPUT;
        head_p->argv_index = argv_index;
        head_p->enabled = ON;
    }
}

void param_create_output(arg_t *head, int argv_index){
    if(!param_status_output(head)){
        arg_t *head_p = head;
        add_to_end(head_p);
        while(head_p->next != NULL){
            head_p = head_p->next;
        }
        head_p->type = OUTPUT;
        head_p->argv_index = argv_index;
        head_p->enabled = ON;
    }
    
}

void param_create_force(arg_t *head){
    if(!param_status_force(head)){
        arg_t *head_p = head;
        add_to_end(head_p);
        while(head_p->next != NULL){
            head_p = head_p->next;
        }
        head_p->type = FORCE;
        head_p->enabled = ON;
        head_p->argv_index = 0;
    }
    
}

void param_create_previous(arg_t *head){
    if(!param_status_previous(head)){
        arg_t *head_p = head;
        add_to_end(head_p);
        while(head_p->next != NULL){
            head_p = head_p->next;
        }
        head_p->type = PREVIOUS;
        head_p->enabled = ON;
        head_p->argv_index = 0;
    }
}