                                 /* resched.c  -  resched */
#include <stdio.h>
#include <proc.h>
#include <signal.h>

void handler();
/*------------------------------------------------------------------------
 * resched  --  find a live thread to run
 *
 *------------------------------------------------------------------------
 */
void resched()
{
//    signal(SIGALRM, handler);  // signal
    register struct  xentry  *cptr;  /* pointer to old thread entry */
    register struct  xentry  *xptr;  /* pointer to new thread entry */
    int i,next;
/* keyword register: register hints the compiler that the given variable 
 * can be put in register. It is the register's choice to put in a refister
 * or not. Generally, compilers do optimizations and put the var in register
 * register is faster than memory
 * don't use "&var", because var is in register other than memory.
 */

    cptr = &xtab[currxid];  /* get current thread's address of PCB/Xtab */

    next = currxid ;  /* point to current thread's xid */
    /* find the next thread whose state is XREAD and do context switch*/
    for(i=0; i<NPROC; i++) {
        if( (++next) >= NPROC)
             next = 0;
        if(xtab[next].xstate == XREADY) {
            xtab[next].xstate = XRUN;
            xptr = &xtab[next];
            currxid = next;
//            signal(SIGALRM, handler);
            ualarm(20000, 0);  // interrupt every 20 ms after the new thread is running
            printf("in resched(): currxid:{%d} state{%d} \n", currxid, xtab[currxid].xstate); //testing

            ctxsw(cptr->xregs,xptr->xregs);

            //printf("in resched-ctxsw "); //testing

            return;
        }
    }
    printf("XT: no threads to run!\n");
    exit(0);
}


void handler() {
    sigset_t set;  // signal data structure set
    sigemptyset(&set);  /* initilaize the value of the set
                         * the value of 32-bit blocked list => 0:
                         * don' block any types of signal.
                         */

    sigaddset(&set, SIGALRM);  // add SIGALRM to variable set
                               // set bit position of SIGALRM as 1

    // Unblock signal SIGALRM as specified in variable set
    if(sigprocmask(SIG_UNBLOCK, &set, 0) < 0) {
       perror("sigprocmask");
       exit(1);
    }
    printf("in handler(). currxid: %d\n", currxid); //testing

    // change the current thread's state from XRUN to XREADY
    xtab[currxid].xstate = XREADY;
    resched();
}
