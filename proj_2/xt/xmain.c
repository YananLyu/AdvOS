/* both foo() and bar() functions has xthread_yiled() function, so foo and 
 * bar will output in inter leave way.
 */

#include <proc.h>
#include <stdio.h>


/*************    the bellow codes are for part 2    *****************/

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]
int cid = 0; // trac the current xid

void foo() {
	printf("foo begins\n");
	xthread_set_ev(&e);
	printf("foo Done\n");
}

xmain_part2(int argc, char* argv[]) {
	xthread_init_ev(&e);  // the event is set NOT_OCCURRED
	printf("xmain 1 \n");
	int cid = xthread_create(foo, 0, NULL);
	xthread_wait_ev(&e);  // main thread will wait
	printf("main Done\n");
	  
}
/*************    the bellow codes are for part 2    *****************/


/*************    the below code is for part 1    *****************/

#define  INTERUPT_TIMES 10  // suppose the interrupt times is 5;
#define WAITING_TIME 30000000
int xidfoo_part1, xidbar_part1;

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

xmain_part1(int argc, char* argv[])
{
   xidfoo_part1 = xthread_create(foo_part1, 0, NULL);
   xidbar_part1 = xthread_create(bar_part1, 0, NULL);
//   xthread_yield(xidyanan);  
/* if we add this function, xmain thread will yield to yanan thread,
 * xmain thread is still not finished. resched function will consider 
 * three threads, xmain, yanan, foo.
 * If xmain thread don't yield, after it is completed, according to 
 * the stack main build, it will go up, so yanan will be executed.
 */ 
}

/*************    the above codes are for part 1    *****************/
