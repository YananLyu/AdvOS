/* resched.c  -  resched */
#include <stdio.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * resched  --  find a live thread to run
 *
 *------------------------------------------------------------------------
 */
void resched()
{
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
            ctxsw(cptr->xregs,xptr->xregs);
            return;
        }
    }
    printf("XT: no threads to run!\n");
    exit(0);
}


