//  *****  Critical Region  ******
//  lock(&m)
//    critical region
//  unlock(&m)
//  ******************************

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// put in the global, because two threads can change it.
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int g = 0;

void split1(void *arg) {
    int k;
    int i = (int) arg;
    for(k=0; k<2000; k++) {
        pthread_mutex_lock(&m);  // lock(&m)
        g++;   // critical region
        pthread_mutex_unlock(&m);  // unlock(&m)
    }
}

void split2(void *arg) {
    int k;
    int i = (int) arg;

    for(k=0; k<2000; k++) {
        pthread_mutex_lock(&m);
        g--;
        pthread_mutex_unlock(&m);
    }
}