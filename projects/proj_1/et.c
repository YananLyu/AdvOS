#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void start_timer();
float stop_timer();

int n;
char *mem;
long memSize;

void * split() {

	for(int i=0; i<memSize; i++) {
//		mem[i] = 'a';
  	}
}

int main(int argc, char * argv[]) {

	if(argc != 2) {
		printf("Only one argument is required!\n");
		exit(1);
	}

	n = atoi(argv[1]);
	mem = (char*)calloc(n, sizeof(char)*1024);
	long memSize = n*sizeof(char)*1024;
	pthread_t ch1;

	start_timer();

	if((pthread_create(&ch1, NULL, split, NULL)) != 0) {
		printf("Pthread create err!");
		exit(0);
	}

	pthread_join(ch1, NULL);  // wait threads finished

	free(mem);
	
	float elapsed_time = stop_timer();
	printf("The time of thread Creation/Deletion is %f s\n", elapsed_time);
}
