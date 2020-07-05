#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "src/log.c"
#include <ctype.h>

#define DEFAULT 100
#ifndef ERROR_FILE
#define ERROR_FILE "log_2.log"
#endif

typedef int (*fizzbuzz_game)(int, int *, int *, int *, int *);

void empty_buffer(int *buffer, int *begin, int *end)
{
    log_info("Starting clearing buffer!");
    for (int j = *(begin) + 1; j <= *(end); j++)
    {
        if (*(begin + j) == -3)
        {
            printf("FizzBuzz\n");
        }
        else if (*(begin + j) == -2)
        {
            printf("Fizz\n");
        }
        else if (*(begin + j) == -1)
        {
            printf("Buzz\n");
        }
        else
        {
            printf("%d\n", *(begin + j));
        }
        *(begin + j) = 0;
    }
    (*end) = 0;
}

void add_to_buffer(int i, int *buffer, int *begin, int *end, int *max)
{
    if (*(end) >= *(max))
    {
        empty_buffer(buffer, begin, end);
        log_info("Clearing buffer complete!");
    }
    log_debug("%d %d", (*end), i);
    *(buffer + (*end)) = i;
    (*end) += 1;
}

int fizzbuzz(int size, int *buffer, int *begin, int *end, int *max)
{
    for (int i = 1; i <= size; i++)
    {
        if (i % 3 == 0 && i % 5 == 0)
        {
            add_to_buffer(-3, buffer, begin, end, max);
        }
        else if (i % 3 == 0)
        {
            add_to_buffer(-2, buffer, begin, end, max);
        }
        else if (i % 5 == 0)
        {
            add_to_buffer(-1, buffer, begin, end, max);
        }
        else
        {
            add_to_buffer(i, buffer, begin, end, max);
        }
    }
    return size;
}

int main(int arg, char *argv[])
{
    freopen(ERROR_FILE, "w", stderr);
    int iter = DEFAULT;
    if (arg > 1)
    {
        iter = atoi(argv[1]);
        log_info("Specified %d iterations", iter);
    }
    else
    {
        log_warn("Didn't specify number of iterations, using default!");
    }

    int buffer[5];
    int begin = 0;
    int end = 0;
    int max = 4;

    int *buffer_p = &buffer[0];
    int *begin_p = &begin;
    int *end_p = &end;
    int *max_p = &max;

    fizzbuzz_game pointer = fizzbuzz;
    int some_var = pointer(iter, buffer_p, begin_p, end_p, max_p);
    empty_buffer(buffer_p, begin_p, end_p);
    
    log_info("Came to an end. Leaving...");

    return 0;
}