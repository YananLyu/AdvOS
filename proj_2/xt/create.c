#include <stdio.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * userret  --  entered when a thread exits by return
 *------------------------------------------------------------------------
 */
void userret()
{
   xtab[currxid].xstate = XFREE;
   printf("XT: Old threads never die; they just fade away. (id:%d)\n",currxid);
   /* find the next runnable thread to trun */
   resched();
}

static int newxid()
{
    int i, xid;
    static int nextproc =0;
  
    for(i=0; i<NPROC; i++) { /* find a free process entry */
        xid = nextproc;
        if((++nextproc) >= NPROC)
             nextproc = 0;
        if(xtab[xid].xstate == XFREE)
             return(xid);
    }
    printf("Error: run out of process table ! \n"); 
    exit(1);
}

/*------------------------------------------------------------------------
 *  xthread_create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
int xthread_create(int *procaddr,int nargs, int args)
{
    WORD *saddr;              /* stack address */
    WORD *ap;  
    struct xentry *xptr;
    int xid;

    xid = newxid();

    xptr = &xtab[xid];
    xptr->xstate = XREADY;

    saddr = (WORD *) xptr->xbase;

    ap = (&args) + nargs;
    for(; nargs > 0; nargs--)
        *(--saddr) = *(--ap); /* copy args onto new process' stack */
    *(--saddr)  = (int)userret;   /* sooner or later you will be there */     
    *(--saddr)  = (int)procaddr;
    --saddr;           /* for frame ebp; it's not important !? */
    saddr -= 2;        /* 2 words for si and di */
    xptr->xregs[SP] = (int) saddr ;

    return(xid);
}
