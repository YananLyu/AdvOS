#include <stdio.h>
#include <proc.h>

extern void xmain();  /* declare xmain function which is in another file */

struct xentry xtab[10];  /* define global var xtab as an array, size 10 */
int currxid = 0;  /* the first thread's PCB information/Xentry stored in
                   * xtab[xurrxid] => xtab[0].
                   */

void main(int argc, char *argv[])
{
   register struct xentry *xptr;  /* xptr can be put in register in CPU */
   struct xentry m;
   int i;
   int xidxmain;

   /* initialize the xtab/PCB, inlucding xid, stack(xbase, xlimit), state */
   for(i=0 ; i < NPROC; i++){
      xptr = &xtab[i];
      xptr->xid = i;
      xptr->xlimit =  (WORD) malloc(STKSIZE);
      xptr->xbase = xptr->xlimit + STKSIZE - sizeof(WORD);
      xptr->xstate = XFREE;
   }

   /* the first thread runs user's xmain with id 0*/
   xidxmain = xthread_create(xmain, 2, argc, argv);
   xtab[xidxmain].xstate = XRUN;
   ctxsw(m.xregs, xtab[xidxmain].xregs);
   /* never be here */
}


