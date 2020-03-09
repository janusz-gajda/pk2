#include <stdio.h>
#include <string.h>
#include <math.h>

void bin(unsigned n) 
{ 
    if (n > 1) 
        bin(n/2); 

    printf("%d", n % 2); 
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
int ustaw(unsigned n, unsigned masc){
    return n | masc;
}

int main(){
    size_t size = sizeof(int) * 8;
    unsigned n;
    char masc_str[size];
    printf("Skompilowana wersja tego programu zaklada rozmiar typu int rowny %d bity.\n",size);
    printf("Podaj liczbe poczatkowa w systemie dziesietnym ");
    scanf("%d",&n);
    printf("\nPodaj maske, w systemie binarnym: ");
    scanf("%s", masc_str);
    printf("\nPodales liczbe %d, ktorej binarna retperentacja to: ",n);
    bin(n);
    printf("\nPo zastosowaniu maski, osiagamy: ");
    bin(ustaw(n,str_to_int(masc_str)));
    return 0;
}