/* both foo() and bar() functions has xthread_yiled() function, so foo and 
 * bar will output in inter leave way.
 */

#include <proc.h>
#include <stdio.h>

#define  INTERUPT_TIMES 10  // suppose the interrupt times is 5;

int xidfoo, xidbar, xidyanan, xidbinu;
int x=0;
typedef xthread_event_t ;

#define WAITING_TIME 10000000
void foo_part1() {
   
   int i = INTERUPT_TIMES;
   while(i > 0) {
      i--;
      printf("foo: %d\n", INTERUPT_TIMES - i);
      int k = WAITING_TIME;  // context switch approximate 3 times.
      while(k) 
        k--;  // we maybe still in this loop when context switch back.
   }
}

void bar_part1() {
   
   int i = INTERUPT_TIMES;
   while(i > 0) {
     i--;
     printf("bar: %d\n", INTERUPT_TIMES - i);
     int k = WAITING_TIME;
     while(k)
        k--;
   }
}

int foo(int f)
{
   int i;
   for(i=0;i<100;i++){
      printf("This is foo %d, %d\n", f, x++);
//      xthread_yield(xidbar);  // don't need it anymore
   }
}

int bar(int p, int q)
{
   int j;
   for(j=0;j<100;j++){
      printf("This is bar %d, %d\n", p-q, x++);
//      xthread_yield(xidfoo);  // don't need it anymore
   }
}

xmain_part1(int argc, char* argv[])
{
   xidyanan = xthread_create(foo_part1, 0, NULL);
   xidbinu = xthread_create(bar_part1, 0, NULL);
//   xthread_yield(xidyanan);  
/* if we add this function, xmain thread will yield to yanan thread,
 * xmain thread is still not finished. resched function will consider 
 * three threads, xmain, yanan, foo.
 * If xmain thread don't yield, after it is completed, according to 
 * the stack main build, it will go up, so yanan will be executed.
 */ 
}
