#include <stdio.h>

int main(int argc, char **argv)
{   int seed;
    if (argc < 2 ) { printf("need a seed!\n"); }
    else {
         seed = atoi(argv[1]);
         applepie("CIS 620 Homework 1 Spring 2019",seed);
    }
}

