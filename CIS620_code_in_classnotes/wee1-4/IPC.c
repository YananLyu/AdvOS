#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t cpid;
    int i, j;
    int g[8192];

    if((cpid = fork()) == 0) { //child
        for(i=0; i<200; i++) {
            printf("Child %d  %d\n", cpid, i);
        }
        // g[0] = g[0] * 2;
        printf("g[0] is %d\n", g[0]);
        exit(0);  // 0 means normal status, 1 means abnormal.
    }

    // parent process
    for(i=0; i<200; i++) {
        printf("Parent %d  %d\n", cpid, i);
    }
}
