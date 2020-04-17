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
typedef struct list *node;
struct list{
    char boolean[9];
    struct list *pNext;
    struct list *pPrevious;
};

node create_first(char *str, node head){
    head = (node)malloc(sizeof(struct list));
    head->pNext = NULL;
    head->pPrevious = NULL;
    strcpy(head->boolean, str);
    return head;
}
node create_next(char *str, node head){
    head->pNext = (node)malloc(sizeof(struct list));
    head->pNext->pPrevious = head;
    head = head->pNext;
    strcpy(head->boolean, str);
    head->pNext = NULL;
    return head;
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
    //Zapisanie danych z bufora do tablicy
    char array[10][9];
    for(int i = 0; i < 10; i+=1){
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

    //Zapisanie tablicy do listy
    node pHead = NULL;
    pHead = create_first(array[0],pHead);
    node pTemp = pHead;
    node pTail = pHead;

    for(int i = 1; i < 10; i++){
        pTemp = create_next(array[i],pTemp);
        pTail = pTemp;
    }

    char number[9];
    printf("Podaj liczbe w systemie binarnym (max 8 bitow): ");
    scanf("%8s",number);
    pTemp = pHead;
    int number_int = str_to_int(number);
    int count = 0;

    for(int i = 0; i < 10; i+=1){
        if(number_int < str_to_int(pTemp->boolean))
            count += 1;
        pTemp = pTemp->pNext;    
    }
    
    printf("Liczba wiekszych liczb od %s to %d\n\nLiczby w tabeli to:\n",number,count);
    pTemp = pHead;
    for(int i = 0; i < 10; i+=1){
        printf("%s\n",pTemp->boolean);
        pTemp = pTemp->pNext;
    }

    return 0;
}