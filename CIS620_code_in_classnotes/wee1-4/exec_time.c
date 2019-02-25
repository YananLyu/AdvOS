#include <sys/time.h>
#include <math.h>
#include <stdio.h>

// int gettimeofday();
int main() {
    struct timeval start_timer, stop_timer;
    gettimeofday(&start_timer, (struct timezone *) NULL);
    float a[1000];
    int i;
    float elapsed_secs;

    for(i=0; i<1000; i++) {  // assume cpu intensive computation
        a[i] = sqrt(sqrt(a[i]));
    }

    gettimeofday(&stop_timer, (struct timezone *) NULL);   // snapshot the current time

    elapsed_secs = (stop_timer.tv_sec - start_timer.tv_sec)
                    + (stop_timer.tv_usec - start_timer.tv_usec)/1000000.0;
                    // convert to float type.

    printf("%f - %f   %f\n", (float)stop_timer.tv_usec, (float)start_timer.tv_usec, elapsed_secs);

    return 0;
}