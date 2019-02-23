/*    proc.h    */
#include <stdio.h>
#include <stdlib.h>

typedef int WORD;  /* give a type of new name. So, WORD just like int */

#define PNREGS 5

#define NPROC 10  /* number of processes/Procedures */

#define SP     0  /* Stack  Pointer */

/* state */
#define XFREE    0
#define XREADY   1
#define XRUN     2

/* This is similar to PCB(Process Control Blocks) which is designed
 * to save and restore the information for context switch */
struct xentry  {
       int  xid;
       WORD xregs[PNREGS];    /* save SP */
       WORD xbase;
       WORD xlimit;
       int  xstate;
};

#define STKSIZE 8192   /* stack size */

extern struct xentry xtab[];

extern int currxid;

/* keyword extern: 1) only declare the variable is gobal
 *                 2) will be defined when it is initilaliztion at first time.
 *                 3) the default value is 0
 *                 4) lifetime: till the end of the execution of program
 *                 5) Scope: not bound by any function. they are everywhere in the program.
*/
