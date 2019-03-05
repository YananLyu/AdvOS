#include <proc.h>

#define NOT_OCCURRED false
#define OCCURRED true

// your code will not call xtread_yield() and ctxsw()
// just use resched()

void xthread_init_ev(struct xthread_event_t * ep) {
	ep->occurred = NOT_OCCURRED;
}

void xthread_wait_ev(struct xthread_event_t * ep) {

	int remain = ualarm(0, 0);  // disable time interrupt

	if(ep->occurred == NOT_OCCURRED) {  // if event is not occurred, 
		ep->Q[qi++] = currxid;   // insert/put the current thread into the wait Q
		isEmpty = false;
             
		xtab[currxid].xstate = XEVENT;  // change the current thr's state = XEVENT
		printf("\n ******in wait_ev of qi: {%d}; waitID: {%d};  waitID's state: {%d}\n",qi, ep->Q[qi], xtab[ep->Q[qi]].xstate ); //for testing
		resched();  // this thr will be back to ready state when someone call it
		return;
	} else {
		ep->occurred = NOT_OCCURRED;
	}

	ualarm(remain);  // enable timer at ending;
}

void xthread_set_ev(struct xthread_event_t * ep) {
	
	if(isEmpty == false) {  // ep->Q is not empty
		isEmpty = true;
		ep->occurred = NOT_OCCURRED;
		printf("\n ******in set_ev of waitID {%d} is waitID's state {%d}\n", ep->Q[qi], xtab[ep->Q[qi]].xstate ); //for testing
		while(qi >= 0) {
			xtab[ep->Q[qi]].xstate = XREADY;
			ep->Q[qi] = -1;  // set Q[qi] from XEVENT to XREADY and remove the thr's id in Q
                        qi--;
		}
		qi = 0;  // qi will be -1 after while loop, so revise it to 0;
	} else {  // if the Q is empty, the event will be set as OCCURRED
		ep->occurred = OCCURRED;
	}
}
