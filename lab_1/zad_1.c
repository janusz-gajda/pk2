#include <stdio.h>

void zad_1(int liczba){
    if(liczba != 0){
        zad_1(liczba/2);
        if(liczba % 2 == 1 || liczba % 2 == -1){
            printf("%d", 1);
        }else{
            printf("%d", 0);
        }
    }
}

int main() {

    int tablica[20];
    int i;
    printf("Podaj 20 licz calkowitych\n");
    for(i = 0; i < 20; i += 1){
        printf("%d. ", i+1);
        scanf("%d", &tablica[i]);
    }
    for(i = 0; i < 20; i += 1){
        printf("%d. ", i+1);
        if(tablica[i] < 0){
            printf("-");
        }
        zad_1(tablica[i]);
        printf("\n");    
    }

    return 0;
}

