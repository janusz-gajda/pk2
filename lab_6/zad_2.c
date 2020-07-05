#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "src/log.c"
#include <ctype.h>

#define MAX_SIZE 100
#ifndef ERROR_FILE
#define ERROR_FILE "log.log"
#endif

int main(){
    for(int i = 1; i <= MAX_SIZE; i++){
        if(i % 3 == 0 && i % 5 == 0){
            printf("FizzBuzz\n");
        }else if(i % 3 == 0){
            printf("Fizz\n");
        }else if(i % 5 == 0){
            printf("Buzz\n");
        }else{
            printf("%d\n",i);
        }
    }
    return 0;
}