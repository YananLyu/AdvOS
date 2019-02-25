#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cnt() {

    static int count = 0;  /* just initialize at first time */
    printf("Interrupt received %d\n", count++);
    alarm(3);  /* the alarm in another function 
                  can trigger signal(SIGALRM, cnt) in main function*/
}

int main() {

    signal(SIGALRM, cnt);
    alarm(3);  /* after 3 senconds, send a alarm signal */
    printf("Begin ... \n");
    for( ; ; )   /* inifinite loop. equal to while(1) 
                     Make sure the process is still running */
        pause();
        
    return 0;
}
