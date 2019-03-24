/* 1) xmain thread call xthread_set_ev()
 *    no process in Queue, so xmain will continue run and set e.occurred as OCCURRED(1)
 *    SO xmain is first done
 * 2) foo thread will run after xmain nature return.
 *    Because e.occurred is OCCURRED(1) before calling xthread_wait_ev() 
 *    the xthread_wait_ev() will set e.occurred as NOT_OCCURRED(0), and foo continue run.
 *    So foo Done and nature return
 * 3) oof thread will run after foo nature return
 *    because e.occurred is NOT_OCCURRED(0) before calling xthread_wait_ev() 
 *    So oof thead will be set as XEVENT, and resched() pick up next XREADY thread
 *    Therefore, oof is not done and no nature return
 * 4) oof1 and oof2 will run and inserted to Q,
 *    like off, they are not Done and no nature return.
 * 5) bar thread is picked up by resched() and will run
 *    it print the Q[qi]'s state(oof), which is XEVENT.
 *    Then call xthread_set_ev(), oof, oof1 and oof2 are in Queue, and e.occurred is NOT_OCCURRED,
 *    so all threads in Q are set as XREADY 
 * 6) Because the state of oof, oof1 and oof2 are XREADY now, so they will be Done and nature return
 */

#include <proc.h>
#include <stdio.h>

struct xthread_event_t e = {.isEmpty = true, .qi = 0};

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
	xthread_wait_ev(&e);  // because NOT_OCCURRED, so oof is inserted to Q
	
	printf("oof Done\n");
}

void oof1() {

	printf("oof1 begins\n");

	printf("oof1-Before call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	xthread_wait_ev(&e);  // because NOT_OCCURRED, so oof is inserted to Q
	
	printf("oof1 Done\n");
}

void oof2() {

	printf("oof2 begins\n");

	printf("oof2-Before call xthread_wait_ev(&e) e.OCCURRED: %d\n", e.occurred);
	xthread_wait_ev(&e);  // because NOT_OCCURRED, so oof is inserted to Q
	
	printf("oof2 Done\n");
}

void bar() {

	printf("bar begins\n");
	
	printf("bar-Before set_ev  e.Q[%d]: %d state: %d\n", e.qi-1, e.Q[e.qi-1],xtab[e.Q[e.qi-1]].xstate);  // XEVENT 3
	xthread_set_ev(&e);   // change the state of threads in Q from XEVEBNT to XREADY
                              // so oof can run and reach nature return.
	printf("bar-After  set_ev  e.Q[%d]: %d state: %d\n", e.qi-1, e.Q[e.qi-1],xtab[e.Q[e.qi-1]].xstate);  // XREADY 1

	printf("bar Done\n");
}

void xmain() {
    printf("Starting with xmain_part2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED

    int usec = ualarm(0,0);  // disable the interrupt
    xthread_create(foo, 0, NULL);
    xthread_create(oof, 0, NULL);
    xthread_create(oof1, 0, NULL);
    xthread_create(oof2, 0, NULL);
    xthread_create(bar, 0, NULL);
    ualarm(usec, 0);  // enbale the interrupt
    
    printf("xmain-Before call xthread_set_ev(&e) e.OCCURRED: %d\n", e.occurred);
    xthread_set_ev(&e);  // no queue process, so xmain will done before foo begins
    printf("xmain-After call xthread_set_ev(&e) e.OCCURRED: %d\n", e.occurred);      

    printf("xmain Done\n");
}
