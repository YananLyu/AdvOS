#include <stdio.h>
#include <proc.h>

/*---------------------------------------------------
 *  xthread_yield  -  yield control to a thread 
 *---------------------------------------------------
 */
void xthread_yield(int xid) /* xid will get the control, that is, xid thread
                             * will be executed instead of the current thread
                             */
{
    struct xentry *cptr,*xptr;

    cptr = &xtab[currxid];  /* cptr point to the current PCB/Xentry */
    cptr->xstate = XREADY;  /* set state of current thr: XRUN -> XREADY */
    xptr = &xtab[xid];
    xptr->xstate = XRUN;  /* set the thread which get control to XRUN */

    currxid = xid;  /* update currxid(globla varibale) to be the new xid */

    ctxsw(cptr->xregs,xptr->xregs);  /* context switch: Actual yield ctrl */ 
}

