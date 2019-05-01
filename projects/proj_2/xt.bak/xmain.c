#include <stdio.h>


int xidfoo, xidbar;
int x=0;

int foo(int f)
{
   int i;
   for(i=0;i<100;i++){
      printf("This is foo %d, %d\n", f, x++);
      xthread_yield(xidbar);
   }
}

int bar(int p, int q)
{
   int j;
   for(j=0;j<100;j++){
      printf("This is bar %d, %d\n", p-q, x++);
      xthread_yield(xidfoo);
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   xthread_yield(xidfoo);
}


