#include <stdio.h>
#include <string.h>

const int N = 100000;
void applepie(char *h,int seed)
{   
    char header[24];
    double x,y; 
    int i, count=0;
    srand48(seed);
    for(i=0; i<N; i++ ) {
         x= drand48();
         y= drand48();
         if(x*x+y*y < 1) count++;
    }
    strcpy(header,h);
    printf("%s \n applepie = %f\n", header, count / N * 4.0);
}

