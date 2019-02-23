/*    proc.h    */
#include <stdio.h>
#include <stdlib.h>

typedef int WORD;

#define PNREGS 5

#define NPROC 10

#define SP     0

/* state */
#define XFREE    0
#define XREADY   1
#define XRUN     2

struct xentry  {
       int  xid;
       WORD xregs[PNREGS];    /* save SP */
       WORD xbase;
       WORD xlimit;
       int  xstate;
};

#define STKSIZE 8192 

extern struct xentry xtab[];

extern int currxid;

