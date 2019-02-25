#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g = 0;   // global shared var
void * split1() {
    int k;
    for(k=0; k<2000; k++) {
        printf("Child thr1:  k:%d  g:%d\n",k, g++);
        fflush(stdout);
    }
    return 0;
}

void * split2() {
    int k;
    for(k=0; k<2000; k++) {
        printf("Child thr2:  k:%d  g:%d\n",k, g++);
        fflush(stdout);
    }
    return 0;
}

int main() {
    pthread_t ch1, ch2;
    if(pthread_create(&ch1, NULL, split1, NULL) != 0) 
        printf("Pthread create err!\n");
    if(pthread_create(&ch2, NULL, split2, NULL) != 0) 
        printf("Pthread 2 create err!\n");
    pthread_join(ch1,NULL);  // wait thread 1 finish
    pthread_join(ch2, NULL);

    pid_t cpid;
    cpid = fork();
    if(cpid < 0) {
        printf("Cannot fork");
        exit(1);
    }
    if(cpid == 0) {  // child 
        printf("This is Child process\n");
        exit(0);
    } else {
        printf("This is Parent process\n");
    }
}