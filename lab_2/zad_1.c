#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
void bin(unsigned n, char* str)
{
    
    if (n > 1)
        bin(n/2,str);

    char zero = '0';
    char one = '1';
    if(n % 2 == 0){
        strncat(str,&zero,1);
    }
    else{
        strncat(str,&one,1);
    }
    
}
int str_to_int(char* str){
    strrev(str);
    int i = 0;
    int sum = 0;
    while(str[i] != '\0'){
        if(str[i] == '1'){
            sum += pow(2,i);
        } 
        i += 1;
    }
    return sum;
}

int main(){
    srand(time(0));
    char buffer[10][9];
    int begin = 0;
    int end = 0;
    int max = 9;
    //Zapełnienie bufora
    while(1){
        char str[9] = "";
        bin(rand()%255,str);
        strcpy(buffer[end],str);
        end+=1;
        if(end >= max+1)
            break;
    }
    printf("\n\n");
    //Zapisanie danych z bufora do tablicy
    char array[255][9];
    for(int i = 0; i < 255; i+=1){
        strcpy(array[i],buffer[begin]);
        char str[9] = "";
        bin(rand()%255,str);
        strcpy(buffer[begin],str);
        begin+=1;
        end+=1;
        if(begin > 9)
            begin = 0;
        if(end > 9)
            end = 0;
    }

    return 0;
}