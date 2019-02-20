#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N   240
// int numOfRow = 0;
int groupSize = 0;
// pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  // mutex
pthread_barrier_t barrier;  // barrier

int * m1;
int * m2;
int * m3; 
//int m3[N][N] = {{0}};

void start_timer();
float stop_timer();

int * matrix_produce() {
	int row = N;
	int col = N;

	int *mat = (int *)calloc(row * col, sizeof(int));
	// set the seed
	srand((unsigned)time(NULL));
	
	int r;
	int c;
	for(r=0; r<row; r++) {
		for(c=0; c<col; c++) {
			int offset = r*col+c;
			mat[offset] = rand()%10;	
		}
	}

	return mat; 
}

int sum(int i, int j) {
	
	int sumIJ = 0;
	int k;
	for(k=0; k<N; k++) {
		sumIJ = m1[i*N+k] * m2[k*N+j];
	}

	return sumIJ;
}

void * calculate(void * startRow_ptr) {
	int i = *(int*) startRow_ptr;
	int j = 0;
	// proctect this area
//	pthread_mutex_lock(&m);
//	int i = numOfRow;
//	numOfRow += groupSize;
//	pthread_mutex_unlock(&m);

	// calculate the matrix
	int max = i + groupSize;
	for(i; i<max; i++) {
		for(j=0; j<N; j++) {
			int offset = i*N+j;
			m3[offset] = sum(i,j);
//			m3[i][j] = sum(i, j);
		}
	}
	pthread_barrier_wait(&barrier);
}

void main(int argc, char * argv[]) {

	if(argc != 2) {
		printf("Only one arguments is required!\n");
		printf("The argumet declare the number of threads\n");

		exit(1);
	}
	
	int numOfThreads = atoi(argv[1]);
	groupSize = N/numOfThreads;

	int j = 0;
        // create three matrix, two are initialed with random value, one is initilazed with 0/
	m1 = matrix_produce();
	m2 = matrix_produce();
	m3 = calloc(N*N,sizeof(int));	

	pthread_barrier_init(&barrier,NULL,numOfThreads+1);
	start_timer();  // start to record running time

	pthread_t ch[numOfThreads];  // create n child thread pointer
	int startRow[numOfThreads];
	int curRow = 0;
	int i;
	for(i = 0; i < numOfThreads; i++) {
		startRow[i] = curRow;
		curRow = curRow + groupSize; 
		if((pthread_create(&ch[i], NULL, calculate, &startRow[i])) != 0) {
			printf("Pthread create err!\n");
			exit(0);
		}
	}

	pthread_barrier_wait(&barrier);
	float elapsed_time = stop_timer();
	pthread_barrier_destroy(&barrier);
	free(m1);
	free(m2);
	free(m3);
	printf("running time is %f\n",elapsed_time);
}
