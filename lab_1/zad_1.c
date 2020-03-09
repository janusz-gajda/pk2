#include <stdio.h>

void bin(unsigned n) 
{ 
    if (n > 1) 
        bin(n/2); 

    printf("%d", n % 2); 
} 

int main() {

    int tablica[20];
    int i;
    printf("Podaj 20 licz naturalnych\n");
    for(i = 0; i < 20; i += 1){
        printf("%d. ", i+1);
        scanf("%d", &tablica[i]);
    }
    for(i = 0; i < 20; i += 1){
        printf("%d. ", i+1);
        bin(tablica[i]);
        printf("\n");    
    }

    return 0;
}

