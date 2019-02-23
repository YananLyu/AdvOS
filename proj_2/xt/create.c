#include <stdio.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * userret  --  entered when a thread exits by return
 *------------------------------------------------------------------------
 */
/* When the thread completed its work(code), it will return or exit
 * This function is designed to set the state of this thread to XFREE when it
 * finished its job
 */
void userret()
{
   xtab[currxid].xstate = XFREE;  /* set the thread to be XFREE state, cause
                                   * the thread have exited */
   printf("XT: Old threads never die; they just fade away. (id:%d)\n",currxid);
   /* find the next runnable thread to trun */
   resched();  /* reschedule next thread to use cpu */
}

/* if a new task/job/work is needed to proceeded, it assign a thread to
 * proceed the task. In order to assign a thread, we firstly need to know
 * which thread is available by checking if its state is XFREE or not. Then
 * return its xid
 */
/* get the id of thread whose state is XFREE  */
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

/* nargs: number of arguments;    
 * args: arguments  
 */
int xthread_create(int *procaddr,int nargs, int args)  
{
    WORD *saddr;              /* stack address */
    WORD *ap;  
    struct xentry *xptr;
    int xid;    /* thread id  */

    xid = newxid();  /* get a thread id for the new thread. if no available
                      * thread resource, it will print error */

    /* when we create a new thread, we should initialize its PCB information
     * for context switch. In our project, data structure Xentry is PCB,
     * which is defined in proc.h file
     */
    xptr = &xtab[xid];  /* xtab is global variable, who is declared in proc.h
                         * file. And its data structure is xentry. As
                         * "extern" mentioned, xtab will be defined when it
                         * is used at first time, let "xentry" pointer xptr 
                         * point to the xtab by assign the first address of
                         * xtab to xptr 
                         */
    xptr->xstate = XREADY;  /* initialize the state to XREADY */

    saddr = (WORD *) xptr->xbase;  /* initialize the base address of stack
                                    * for the thread  */

    /** Flowing statements are used to initilize the stack of the new thread **/
    ap = (&args) + nargs;  /* ap: args pointer. put the argumets which 
                            * function foo(args)/bar(args) need to the stack
                            * of new thread 
                            */ 
    for(; nargs > 0; nargs--)
        *(--saddr) = *(--ap); /* copy args onto new process' stack */
    *(--saddr)  = (int)userret;   /* sooner or later you will be there */     
    *(--saddr)  = (int)procaddr;
    --saddr;           /* for frame ebp; it's not important !? */
    saddr -= 2;        /* 2 words for si and di */
    xptr->xregs[SP] = (int) saddr ;

    return(xid);
}
