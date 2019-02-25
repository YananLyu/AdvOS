#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int g = 0;
void split(void *arg) {
    int i = (int) arg;
    int k;

    for(k=0; k<2000; k++) {
        printf("Child thr%d:  %d:", i, g++);
        fflush(stdout);
    }

}

void split1(void *arg) {
    int k;
    int i = (int) arg;
    for(k=0; k<2000; k++) {
        g++;
    }
}

void split2(void *arg) {
    int k;
    int i = (int) arg;

    for(k=0; k<2000; k++) {
        g--;
    }
}