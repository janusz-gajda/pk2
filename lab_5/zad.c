#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Do lini poleceń dopisz nazwę pliku txt np: ./zad.exe out.txt

enum MachineState
{
    MS_Stop = 1,
    MS_Start,
    MS_Lift,
    MS_Flip,
    MS_Drop
};

void buffor_is_full(int *buffer, int *begin, int *end, FILE *txt)
{
    for (int i = (*begin); i <= (*end); i++)
    {
        if (*(begin + i) == MS_Start)
        {
            printf("MS_START\n");
            fprintf(txt, "MS_START\n");
        }
        else if (*(begin + i) == MS_Stop)
        {
            printf("MS_STOP\n");
            fprintf(txt, "MS_STOP\n");
        }
        else if (*(begin + i) == MS_Lift)
        {
            printf("MS_Lift\n");
            fprintf(txt, "MS_Lift\n");
        }
        else if (*(begin + i) == MS_Flip)
        {
            printf("MS_Flip\n");
            fprintf(txt, "MS_Flip\n");
        }
        else if (*(begin + i) == MS_Drop)
        {
            printf("MS_Drop\n");
            fprintf(txt, "MS_Drop\n");
        }
        *(begin + i) = 0;
    }
    (*end) = 0;
}

void prt(int *crane, int state, int *buffer, int *begin, int *end, int *max, FILE *txt)
{
    if (*(end) >= *(max))
    {
        buffor_is_full(buffer, begin, end, txt);
    }
    *(buffer + (*end)) = state;
    (*end)++;
    (*crane) = state;
};
typedef struct Crane_
{
    int state;
    int *p;
} Crane;

int main(int arg, char *argv[])
{
    if (argv[1])
    {
        FILE *main = fopen(argv[1], "w");
        int buffer[5];
        int begin = 0;
        int end = 0;
        int max = 4;

        int *buffer_p = &buffer[0];
        int *begin_p = &begin;
        int *end_p = &end;
        int *max_p = &max;

        struct Crane_ test;
        test.p = &test.state;
        prt(test.p, MS_Start, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Lift, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Drop, buffer_p, begin_p, end_p, max_p, main);

        prt(test.p, MS_Lift, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Flip, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Drop, buffer_p, begin_p, end_p, max_p, main);

        prt(test.p, MS_Lift, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Drop, buffer_p, begin_p, end_p, max_p, main);

        prt(test.p, MS_Lift, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Flip, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Drop, buffer_p, begin_p, end_p, max_p, main);
        prt(test.p, MS_Stop, buffer_p, begin_p, end_p, max_p, main);

        buffor_is_full(buffer_p, begin_p, end_p, main);

        fclose(main);
    }
    return 0;
}