#include "../include/helpers.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define PATH_IN  "tema3.in"
#define PATH_OUT "tema3.out"

float float_max(float a, float b) {
    return (a > b) ? a : b;
}

int int_max(int a, int b) {
    return (a > b) ? a : b;
}

void reset_output_file()
{
    int f = open(PATH_OUT, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (f == -1)
    {
        printf("Error opening output file\n");
        exit(EXIT_FAILURE);
    }
    close(f);
}

FILE *open_file_in()
{
    FILE *f = fopen(PATH_IN, "r");
    if (f == NULL)
    {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    return f;
}

FILE *open_file_out()
{
    FILE *f = fopen(PATH_OUT, "a");
    if (f == NULL)
    {
        printf("Error opening output file");
        exit(EXIT_FAILURE);
    }
    return f;
}