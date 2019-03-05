/* both foo() and bar() functions has xthread_yiled() function, so foo and 
 * bar will output in inter leave way.
 */

#include <proc.h>
#include <stdio.h>


/*************    the bellow codes are for part 2    *****************/

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = -1;  // track the Q[i]
//int cid = 0; // trac the current xid

/* ------------------------>  test 1  <-----------------------------*/
void foo_part2_test1() {
	printf("foo begins\n");
	xthread_set_ev(&e);
	printf("foo Done\n");
}

void xmain_part2_test1() {
    printf("Starting with xmain_part2_test1\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED
    
    int usec = ualarm(0, 0);  // disable interrupt
    xthread_create(foo_part2_test1, 0, NULL);
    ualarm(usec, 0);  // enable interrupt
    printf("********in xmain_part2: currxid %d\n", currxid);  //testing // don't need cid
    xthread_wait_ev(&e);  // main thread will wait
    
    printf("main Done\n");
}
/* -----------------------------------------------------------------*/

/* ------------------------>  test 2  <-----------------------------*/
int k = 0;
void foo_part2_test2() {
	int id = k++;
	printf("foo %d begins\n", id);
//	xthread_set_ev(&e);
	xthread_wait_ev(&e);  // thread will wait
	printf("foo %d Done\n", id);
}

void xmain_part2_test2() {
    printf("Starting with xmain_part2_test2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED

    int usec = ualarm(0,0);  // disable the interrupt
    xthread_create(foo_part2_test2, 0, NULL);
    xthread_create(foo_part2_test2, 0, NULL);
    xthread_create(foo_part2_test2, 0, NULL);
    xthread_create(foo_part2_test2, 0, NULL);
    xthread_create(foo_part2_test2, 0, NULL);
    xthread_create(foo_part2_test2, 0, NULL);
    ualarm(usec, 0);  // enbale the interrupt
    
//    printf("********in xmain_part2: currxid %d\n", currxid);  //testing // don't need cid
    xthread_wait_ev(&e);  // main thread will wait

    xthread_set_ev(&e);
    printf("main Done\n");
}
/* -----------------------------------------------------------------*/
/*************    the above codes are for part 2    *****************/


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
