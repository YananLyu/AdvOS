#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void start_timer();
float stop_timer();

int main(int argc, char * argv[]) {

	if(argc != 2) {
		printf("Only one argument is required!\n");
		exit(1);
	}

	int n = atoi(argv[1]);
	
	pid_t cpid;
	int status;
	char *mem;

	mem = (char*)calloc(n, sizeof(char)*1024);  // allocate dynamic memeory
	long memSize = n*sizeof(char)*1024;

	start_timer();
	if((cpid = fork()) == 0) {  // child process
		for(int i=0; i<memSize; i++) {
//			mem[i] = 'a';
		}
		
		exit(0);
	}

	// parent process
	waitpid(cpid, &status, 0);   // parent process should wait child process
	
	free(mem);  // free the dynamic memory

	float elapsed_time = stop_timer();

	printf("The runtime of process creation/deletion is %f s.\n",elapsed_time);
	
	return 0;
}
