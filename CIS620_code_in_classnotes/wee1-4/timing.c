#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void start_timer();
float stop_timer();

struct timeval s_timer, e_timer;
int main() {
    start_timer();

    int i;
    for(i=0; i<5; i++) {
        sleep(1);
    }

    float time = stop_timer();
    printf("%f", time);
}

void start_timer() {
    gettimeofday(&s_timer, (struct timezone *) NULL);
}

float stop_timer() {
    gettimeofday(&e_timer, (struct timezone *) NULL);
    float elapsed_secs = (e_timer.tv_sec - s_timer.tv_sec)
                         + (e_timer.tv_usec - s_timer.tv_usec) / 1000000.0;
    return elapsed_secs;
}

