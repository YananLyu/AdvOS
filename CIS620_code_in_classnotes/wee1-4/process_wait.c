#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    
    int status;
    pid_t cpid;
    int i;
    if((cpid = fork()) == 0) {
        printf("This is child process\n");
        sleep(5);  // sleep 10 seconds
        exit(2);
    }

    // parent process
    // p will wait until child finish
    waitpid(cpid, &status, 0);
    printf("This is parent process\n");

    return 0;
}