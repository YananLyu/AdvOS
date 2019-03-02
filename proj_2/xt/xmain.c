/* both foo() and bar() functions has xthread_yiled() function, so foo and 
 * bar will output in inter leave way.
 */

#include <proc.h>
#include <stdio.h>
// add header files for signal
#include <signal.h>
#include <unistd.h>



void handler() {
   sigset_t set; // signal data structure set
   sigemptyset(&set); /* initilaize the value of the set
                       * the value of 32-bit blocked list => 0: 
                       * don' block any types of signal.
                       */
   sigaddset(&set, SIGALRM);  // add SIGALRM to varibale set
   
   // Unblock signal SIGALRM as specified in varibale set
   sigpromask(SIG_UNBLOCK, &set, 0);

   xtab[currxid].xstate = XREADY;
   resched();
   
                       
}


int xidfoo, xidbar;
int x=0;

int foo(int f)
{
   int i;
   for(i=0;i<100;i++){
      printf("This is foo %d, %d\n", f, x++);
      xthread_yield(xidbar);
   }
}

int bar(int p, int q)
{
   int j;
   for(j=0;j<100;j++){
      printf("This is bar %d, %d\n", p-q, x++);
      xthread_yield(xidfoo);
   }
}

xmain(int argc, char* argv[])
{
   signal(SIGALRM, handler);
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   xthread_yield(xidfoo);
}


