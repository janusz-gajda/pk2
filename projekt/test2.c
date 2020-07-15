/*  checkthis.c  */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){
int seed;
time_t tt;
int c = 0;
seed = time(&tt);
srand(seed);
    for(int i = 1; i <= 9; i++){
        for(int j = 1; j <= 9; j++){
            c = rand() % 10;
            print("%i",c);
            if(j%3 == 0){
                print(" ");
            print(" ");
                
            }
                
                    
        }
        if(i%3 == 0)
            print("/n");
        print("/n");    
    }
    
//getch();


return 0;

}