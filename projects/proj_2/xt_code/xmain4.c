#include <proc.h>
#include <stdio.h>

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]


void foo() {
	printf("foo begins\n");
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
    
    printf("main Done\n");
}
