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

		printf("*** wait:  qi:%d; Q[qi]: %d\n", qi-1, ep->Q[qi-1]);

		xtab[currxid].xstate = XEVENT;  // change the current thr's state = XEVENT

		resched();  // this thr will be back to ready state when someone call it
		return;
	} else {  // OCCURRED
		ep->occurred = NOT_OCCURRED;
	}

	ualarm(remain);  // enable timer at ending;
}

void xthread_set_ev(struct xthread_event_t * ep) {
	
	if(isEmpty == false) {  // ep->Q is not empty
		int remain = ualarm(0, 0); 
		ep->occurred = NOT_OCCURRED;
		qi--;
		while(qi >= 0) {
			xtab[ep->Q[qi]].xstate = XREADY;
			ep->Q[qi] = -1;  // set Q[qi] from XEVENT to XREADY and remove the thr's id in Q
                        qi--;
		}
		isEmpty = true;
		qi = 0;  // qi will be -1 after while loop, so revise it to 0;
		remain = ualarm(0, 0); 
	} else {  // if the Q is empty, the event will be set as OCCURRED
		ep->occurred = OCCURRED;
	}
}
