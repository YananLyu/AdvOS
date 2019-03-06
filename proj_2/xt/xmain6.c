/* if there are no Queued processes, the event will be set as OCCURRED 
 * and the 
 */

#include <proc.h>
#include <stdio.h>

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]

void foo() {

	printf("foo begins\n");

	printf("foo-Before call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	xthread_wait_ev(&e);  // cause OCCURRED, so foo continue run
			      // and set e.occurred as NOT_OCCURRED.
	printf("foo-After call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	
	printf("foo Done\n");
}

void oof() {

	printf("oof begins\n");

	printf("oof-Before call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	xthread_wait_ev(&e);  // cause NOT_OCCURRED, so oof is inserted to Q
			      // 
	printf("oof-After call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	
	printf("oof Done\n");
}

void bar() {

	printf("bar begins\n");
	
	printf("bar-Before set_ev  e.Q[%d]: %d state: %d\n", qi-1, e.Q[qi-1],xtab[e.Q[qi-1]].xstate);  // XEVENT 3
	xthread_set_ev(&e);   // change the state of threads in Q from XEVEBNT to XREADY
                              // so oof can run and reach nature return.
	printf("bar-After  set_ev  e.Q[%d]: %d state: %d\n", qi-1, e.Q[qi-1],xtab[e.Q[qi-1]].xstate);  // XREADY 1

	printf("bar Done\n");
}

void xmain() {
    printf("Starting with xmain_part2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED

    int usec = ualarm(0,0);  // disable the interrupt
    xthread_create(foo, 0, NULL);
    xthread_create(oof, 0, NULL);
    xthread_create(bar, 0, NULL);
    ualarm(usec, 0);  // enbale the interrupt
    
    printf("xmain-Before call xthread_set_ev(&e) e.OCCURRED: %d\n", e.occurred);
    xthread_set_ev(&e);  // no queue process, so xmain will done before foo begins
    printf("xmain-After call xthread_set_ev(&e) e.OCCURRED: %d\n", e.occurred);      

    printf("xmain Done\n");
}