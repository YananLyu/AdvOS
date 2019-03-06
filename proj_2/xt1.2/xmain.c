/* both foo() and bar() functions has xthread_yiled() function, so foo and 
 * bar will output in inter leave way.
 */

#include <proc.h>
#include <stdio.h>




#define  INTERUPT_TIMES 10  // suppose the interrupt times is 5;
#define WAITING_TIME 3000000

int xidfoo_part1, xidbar_part1;


struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]
int cid = 0; // trac the current xid




/*************    the bellow codes are for part 1    *****************/



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


/*************    the bellow codes are for part 2    *****************/

void foo_part2() {
    printf("foo begins\n");
    xthread_set_ev(&e);
    printf("foo Done\n");
}

void xmain_part2() {
    printf("Starting with proj _part2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED
    printf("xmain 1 \n");
    int cid = xthread_create(foo_part2, 0, NULL);
    printf("********in xmain_part2: currxid %d, cid: %d\n", currxid, cid);  //testing
    xthread_wait_ev(&e);  // main thread will wait
    printf("main Done\n");
}



/*************   xmain() to call part1 & part 2    *****************/

xmain(int argc, char* argv[])
{
   xidfoo_part1 = xthread_create(foo_part1, 0, NULL);
   xidbar_part1 = xthread_create(bar_part1, 0, NULL);
    xmain_part2();

//   xthread_yield(xid);
/* if we add this function, xmain thread will yield to thread,
 * xmain thread is still not finished. resched function will consider
 * three threads, xmain, yanan, foo.
 * If xmain thread don't yield, after it is completed, according to
 * the stack main build, it will go up, so yanan will be executed.
 */
}


