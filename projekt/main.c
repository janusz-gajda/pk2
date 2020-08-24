//CLI arguments
//-h --help - shows help
//-i --input - input CSV formatted puzzle
//-o --output - save solved puzzle into CSV file
//-p --previous - shows history of puzzles
//-f --force - force solving, even if in hisotry
//[none] - shows help

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "src/generic.h"
#include "src/solving.h"
#include "src/json.h"
#include "src/param.h"

int main(int argc, char *argv[]){
    //if no parameter show help, then exit
    if(argc < 2){
        help();
        return 0;
    }
    
    //create necessary variables
    arg_t *head = param_create_struct();
    char *string_from_file = NULL;
    long fsize;
    
    //read starting params
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")){
            help();
            stop_program(head, string_from_file);
        } else if((!strcmp(argv[i],"-i") || !strcmp(argv[i],"--input"))){
            param_create_input(head,++i);
        } else if((!strcmp(argv[i],"-o") || !strcmp(argv[i],"--output"))){
            param_create_output(head,++i);
        } else if(!strcmp(argv[i],"-p") || !strcmp(argv[i],"--previous")){
            param_create_previous(head);
        } else if(!strcmp(argv[i],"-f") || !strcmp(argv[i],"--force")){
            param_create_force(head);
        }
    }

    //read history file
    string_from_file = start_history(head, &fsize);

    //if previous, show previous results, then exit
    if(param_status_previous(head)){
        start_previous(head, string_from_file, fsize);
        stop_program(head, string_from_file)
    }
    
    //if input, start solver
    if(param_status_input(head)){
        start_solver(head, string_from_file, fsize+1, argv);
    } 

    stop_program(head, string_from_file);
    
}