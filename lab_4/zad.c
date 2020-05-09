#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define INITIAL_CAPACITY  10

enum state {
    begin,
    inside_tag,
    reading_markup
};

void push(char *arr, int index, char value, int *size, int *capacity){
    if(*size > *capacity){
        realloc(arr, sizeof(arr) * 2);
        *capacity = sizeof(arr) * 2;
    }
    arr[index] = value;
    *size = *size + 1;
};

int main (int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");

    int f_capacity = INITIAL_CAPACITY;
    int s_capacity = INITIAL_CAPACITY;
    int f_size = 0;
    int s_size = 0;
    int f_index = 0;
    int s_index = 0;
    char *first_time = malloc(INITIAL_CAPACITY * sizeof(char));
    char *second_time = malloc(INITIAL_CAPACITY * sizeof(char));
    int state = begin;
    char ch;

    while((ch = fgetc(input)) != EOF){
        if(ch == '\n' || ch == '\t' || ch == ' ' || ch == '\v' || ch == '\f' || ch == '\r'){
            continue;
        }
        if(state == begin){
            if(ch == '<'){
                state = inside_tag;
                continue;
            } else{
                continue;
            }
        } else if(state == inside_tag){
            if(ch == '>'){
                state = reading_markup;
                continue;
            } else{
                continue;
            }
        } else if(state == reading_markup){
            if(ch == '<'){
                state = inside_tag;
                continue;
            } else{
               int is_in_first = 0, is_in_second = 0;
               for(int i = 0; i < f_index; i++){
                   if(first_time[i] == ch){
                       is_in_first = 1;
                       break;
                   }
               }
               for(int i = 0; i < s_index; i++){
                   if(second_time[i] == ch){
                       is_in_second = 1;
                       break;
                   }
               }
               if(is_in_first == 0 && is_in_second == 0){
                   push(first_time,f_index,ch,&f_size,&f_capacity);
                   f_index++;
                   continue;
               } else if(is_in_first == 1 && is_in_second == 0){
                   push(second_time,s_index,ch,&s_size,&s_capacity);
                   s_index++;
                   continue;
               }
            }
        }
    }

    fclose(input);

    int found = 0;
    for(int i = 0; i < f_index; i++){
        int is_in_second = 0;
        for(int j = 0; j < s_index; j++){
            if(first_time[i] == second_time[j]){
                is_in_second = 1;
                break;
            }
        }
        if(is_in_second == 0){
            found = 1;
            printf("Pierwszy znakiem unikalnym jest: %c",first_time[i]);
            break;
        }
    }

    if(!found){
        printf("Nie ma znakow unikalnych");
    }
    return 0;
}