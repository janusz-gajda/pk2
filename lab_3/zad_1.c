#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int com (const void *p, const void *q){
    return ( *(int*)p - *(int*)q);
}
int main(){
    srand(time(0));
    //qsort
    printf("qsort\n");
    int array[10];
    for(int i = 0; i < 10; i++){
        array[i] = rand();
        printf("%d ",array[i]);
    }
    printf("\n");
    qsort(array,10,sizeof(int),com);
    for(int i = 0; i < 10; i++)
        printf("%d ",array[i]);
    //memcpy
    {
        char str1[] = "Ala ma kota";
        char str2[] = "Ula ma psa";
        printf("\n\nmemcpy\n%s\n%s\n\n",str1, str2);
        memcpy(str1,str2,3); 
        printf("%s\n",str1);
    }
    //memmove
    {
        char str1[] = "Ala ma kota";
        char str2[] = "Ula ma psa";
        printf("\nmemmove\n%s\n%s\n\n",str1, str2);
        memmove(str1,str2,3); 
        printf("%s\n",str1);
    }
    //sprintf
    {
        char buffer[50]; 
        int a = 10, b = 20, c; 
        c = a + b; 
        sprintf(buffer, "Suma  %d i %d to %d", a, b, c); 
    }
    //puts fputs
    {
        puts("\nHello World");
        fputs("\nHello World",stdout);
    }
    //isdigit
    {
        char c = '6';
        printf("\nWartoś numertczna %c to %d\n",c,isdigit(c));
    }
    //system 
    {
        system("dir");
    }
    //strcat
    {
        char a[60] = "Ala ma kota";
        char b[] = "Ula ma psa";
        char c = 'l';
        strcat(a,b);
        strcspn(a,c);
    }
    //strdup
    {
        char a[] = "Ala ma kota";
        char *kopia = strdup(a);
    }
}