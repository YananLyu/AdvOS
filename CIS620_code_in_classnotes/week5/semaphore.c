//
//  semaphore.c
//  
//
//  Created by Yanan Lyu on 2/13/19.
//

/*******     For Single core CPU     ********/

#include <stdlib.h>

/*  DOWN  */
void sem_wait(sem_t *s) {  // s->count
    
    disable();  // disable interupt
    s->count--;  // means read subtract write
    
    if(s->count < 0) {  // need to wait
        // enqueue the calling thread in a waiting Q, s->Q
        reschedule();  // OS function. OS finds next READY thread to run
    }
    
    enable();
}

/*  UP  */
void sem_post(sem_t *s) {
    
    disable();
    s->count++;
    
    if (s->Q is NOT empty) {
        tid = dequeue(s->Q);
        put tid to READY list/ put tid to READY
        
        reschedule();
    }
    
    enable();
}

/* initial value for semaphore is important */
globalsem_t s  // global variable
sem_init(&s,0, 1)  // 0 means only shared among threads
                   // 1 : initial counter value to 1
sem_wait(&s);  // s->count = 0
    enter C.R.
sem_wait(&s);  // s->count = -1
    can't enter C.R
sem_post(&s);  // s->count = 0

// if s->count < 0
//      |s->count|: # of waiting in Q


/******    MULTIPROCESSOR    ******/
// TAS: Test And Set   like tsc
//       hardware  read value and set to 1;
// when the value in hardware is read, at the same time, it is set to 1
void sem_wait(sem_t *s) {
    
    disable();
    while(TAS(&v) != 0);  // Lock. Busy waiting
    
    s->count--;
    
    if(s->count < 0) {  // need to wait
        enqueue the calling thread in a waiting enqueue Q
        reschedule();
    }
    
    v = 0;  // unlock
    enable();
}
