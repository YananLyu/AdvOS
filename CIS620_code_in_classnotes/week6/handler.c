#include <signal.h>
#include <setjmp.h>
#include <stdio.h>

sigjmp_buf env;

void handler() {

    sigset_t set;
    sigemptyset(&set);  /* initialize the value of set is 0, that is, empty it */
    sigaddset(&set, SIGALRM);  /* add signal SIGALRM to signal set
                                * we will use the "set" to block or ublock signals sepecified by "set"
                                */

    /* ublock these signals in set. the signals in set will not be blocked.
     * three methods to change mask: SIG_SETMASK, SIG_BLOCK, SIG_UNBLOCK
     * SIG_SETMASK: set these signals should be blocked
     * SIG_BLOCK: add these specified sigbals to the old blocked_set. these signals will be blocked.
     * SIG_UBLOCK: remove these signals from blocked_set. these signall won't be blocked.
     */
    sigprocmask(SIG_UNBLOCK,&set, 0);

    /* the following is what the handler should do to deal with the signal */
    printf("3 sec \n");  //  do what we want to do
    /* use following to instead of print()
    xtab[curcid].xstate = XREADY;
    resched();
    */

    /* after tackling the signal, jump back to where we hope it run from. */
    longjmp(env,1);

}
