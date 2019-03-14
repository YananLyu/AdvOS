/* foo done before xmain done, because xmain thread is inserted to Queue
 * and state is set as XEVENT. when foo is in busy waiting loop and a new
 * signal arrives, xmain thread cannot run because it's in XEVENT state other than XREADY state. 
 * so xmain thread is still Done after foo Done 
*/

#include <proc.h>
#include <stdio.h>

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]


void foo() {
	printf("foo begins\n");
	int i;
	for(i=0; i<20000000; i++);  // busy waiting for new signals
	xthread_set_ev(&e);
	printf("foo Done\n");
}

void xmain() {
    printf("Starting with xmain_part2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED
    
    int usec = ualarm(0, 0);  // disable interrupt
    xthread_create(foo, 0, NULL);
    ualarm(usec, 0);  // enable interrupt
    
    xthread_wait_ev(&e);  // xmain thread will be inserted to Queue
    
    printf("xmain Done\n");
}
