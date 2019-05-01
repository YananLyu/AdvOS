#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int myrank, nprocs;

    char hostname[256];
    gethostname(hostname, 256);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    printf("Hello from %s processor %d of %d\n", hostname, myrank, nprocs);

    MPI_Finalize();
    return 0;
}
