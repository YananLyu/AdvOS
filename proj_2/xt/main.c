#include <stdio.h>
#include <proc.h>
#include <signal.h>

extern void handler();

extern void xmain_part1();  /* declare xmain function which is in another file */
extern void xmain_part2_test1();  /* declare xmain function which is in another file */
extern void xmain_part2_test2();  /* declare xmain function which is in another file */

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

   // check the argumets
   if(argc < 2) {
      
      printf("Please specify the part you want to run by specifing the second argumet with a number as bellow\n");
      printf("For example:\n./a.out 1\n   or\n./a.out 2\n\n");
      printf("1: part 1\n2: part2_test1\n3: part2_test2\n...\n");
      exit(1);
   }

   /* initialize the xtab/PCB, inlucding xid, stack(xbase, xlimit), state */
   for(i=0 ; i < NPROC; i++){
      xptr = &xtab[i];
      xptr->xid = i;
      xptr->xlimit =  (WORD) malloc(STKSIZE);
      xptr->xbase = xptr->xlimit + STKSIZE - sizeof(WORD);
      xptr->xstate = XFREE;
   }
   
   /* the first thread runs user's xmain with id 0*/
   switch(atoi(argv[1])) {

      case 1:  // part 1
         xidxmain = xthread_create(xmain_part1, 2, argc, argv);  // test xmain_part1
         break;   

      case 2:  // part 2: test 1
         xidxmain = xthread_create(xmain_part2_test1, 2, argc, argv);  // test xmain_part2
         break;

      case 3:  // part 2: test 2
         xidxmain = xthread_create(xmain_part2_test2, 2, argc, argv);  // test xmain_part2
         break;

      default:
         printf("The minimum number is 1\n");
         printf("The maximum number is 2\n");
         exit(1);
   }

   xtab[xidxmain].xstate = XRUN;
   
   signal(SIGALRM, handler);  // it is better to place signal in main funciton.
   currxid = xidxmain;
   ctxsw(m.xregs, xtab[xidxmain].xregs);
   /* never be here */
}
