#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned count = 5;
pthread_barrier_t barrier;  // no pthread_barrier_ t in Mac OS
int main() {
    pthread_barrier_init(&barrier, NULL, count);  // Null it default; 
                                                  //count should consider main(), i.e. 4+1
    pthread_barrier_wait(&barrier);  // main should have pthread_barrier_wait,
                                    // if no, main will run to end
    return 0;
}

void split() {
    computation stage1;
    pthread_barrier_wait(&barrier);  // blocking system call
    computation stage 2 if necessary;
}