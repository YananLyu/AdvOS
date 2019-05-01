#include <proc.h>
#include <stdio.h>

#define  INTERUPT_TIMES 10  // suppose the interrupt times is 5;
#define WAITING_TIME 5000000
int xidfoo, xidbar;

void foo() {
   
   int i = INTERUPT_TIMES;
   while(i > 0) {
      i--;
      printf("foo: %d\n", INTERUPT_TIMES - i);
      int k = WAITING_TIME;  // context switch approximate 3 times.
      while(k) 
        k--;  // we maybe still in this loop when context switch back.
   }
}

void bar() {
   
   int i = INTERUPT_TIMES;
   while(i > 0) {
     i--;
     printf("bar: %d\n", INTERUPT_TIMES - i);
     int k = WAITING_TIME;
     while(k)
        k--;
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 0, NULL);
   xidbar = xthread_create(bar, 0, NULL);
//   xthread_yield(xidyanan);  
/* if we add this function, xmain thread will yield to yanan thread,
 * xmain thread is still not finished. resched function will consider 
 * three threads, xmain, yanan, foo.
 * If xmain thread don't yield, after it is completed, according to 
 * the stack main build, it will go up, so yanan will be executed.
 */ 
}
