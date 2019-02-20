#include <sys/time.h>
#include <math.h>
#include <stdio.h>

struct timeval start_time, stop_time;
void start_timer() {
	gettimeofday(&start_time, (struct timezone *) NULL);	
}

float stop_timer() {
	gettimeofday(&stop_time, (struct timezone *) NULL);
	float elapsed_time = (stop_time.tv_sec - start_time.tv_sec)
			+ (stop_time.tv_usec - start_time.tv_usec)/1000000.0; 
	
	return elapsed_time;
}
