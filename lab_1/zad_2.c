#include <stdio.h>
#include <math.h>

void bin(unsigned n) 
{ 
    if (n > 1) 
        bin(n/2); 

    printf("%d", n % 2); 
}
int ustaw(unsigned n, unsigned position, size_t size){
    if(position > size){
        printf("Podano bledna pozycje");
    }else{
        unsigned masc = pow(2, size - position - 1);
        return (n | masc);
    }
}

int main(){
    size_t size = sizeof(int) * 8;
    unsigned n, position;
    printf("Skompilowana wersja tego programu zaklada rozmiar typu int rowny %d bity.\n",size);
    printf("Podaj liczbe poczatkowa w systemie dziesietnym ");
    scanf("%d",&n);
    printf("\nPodaj pozycje, ktora chcesz ustawic na 1 (0-%d)",size-1);
    scanf("%d",&position);
    printf("\nPodales liczbe %d, ktorej binarna retperentacja to: ",n);
    bin(n);
    printf("\nPo ustawieniu bitu nr %d, uzyskujemy: ",position);
    bin(ustaw(n,position,size));
    return 0;
}