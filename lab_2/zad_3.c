#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
void int_to_bin(unsigned n) 
{ 
    if (n > 1) 
        int_to_bin(n/2); 

    printf("%d", n % 2); 
} 

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
typedef struct list *node;
struct list{
    int value;
    struct list *pNext;
    struct list *pPrevious;
};

node create_first(int val, node head){
    head = (node)malloc(sizeof(struct list));
    head->pNext = NULL;
    head->pPrevious = NULL;
    head->value = val;
    return head;
}
node create_next(int val, node head){
    head->pNext = (node)malloc(sizeof(struct list));
    head->pNext->pPrevious = head;
    head = head->pNext;
    head->value = val;
    head->pNext = NULL;
    return head;
}

int main(){
    srand(time(0));
    int buffer[10];
    int *buffer_p = buffer;
    int begin = 0;
    int end = 0;
    int max = 9;
    //Zapełnienie bufora
    for(int i = 0; i < 10; i += 1){
        *(buffer_p+i) = rand() % 255; 
    }
    //Zapisanie danych z bufora do tablicy
    int array[10];
    int *array_p = array;

    for(int i = 0; i < 10; i+=1){
        *(array_p+i) = *(buffer_p+begin);
        *(buffer_p+begin) = rand() % 255;
        begin+=1;
        end+=1;
        if(begin > 9)
            begin = 0;
        if(end > 9)
            end = 0;
    }

    //Zapisanie tablicy do listy
    node pHead = NULL;
    pHead = create_first(*array_p,pHead);
    node pTemp = pHead;
    node pTail = pHead;

    for(int i = 1; i < 10; i++){
        pTemp = create_next(*(array_p+i),pTemp);
        pTail = pTemp;
    }

    char number[9];
    printf("Podaj liczbe w systemie binarnym (max 8 bitow): ");
    scanf("%8s",number);
    pTemp = pHead;
    int number_int = str_to_int(number);
    int count = 0;

    for(int i = 0; i < 10; i+=1){
        if(number_int < pTemp->value)
            count += 1;
        pTemp = pTemp->pNext;    
    }
    
    printf("Liczba wiekszych liczb od %s to %d\n\nLiczby w tablicy to:\n",number,count);
    pTemp = pHead;
    for(int i = 0; i < 10; i+=1){
        int_to_bin(pTemp->value);
        printf("\n");
        pTemp = pTemp->pNext;
    }

    return 0;
}