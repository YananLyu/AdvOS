#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KEY 7890  /* assume unique known by other process, work together */
#define PERM 0600  /* permission ugo; only owner can rw */

int main() {

    int shm_id;
    int i;
    int * iptr;

    shm_id = shmget(KEY, sizeof(int), PERM | IPC_CREAT);
    iptr = (int *) shmat(shm_id, (char * ) 0, 0);

    for(i=0; i<20; i++) {
        printf("P1 %d\n", (*iptr)++);
        sleep(1);
    }

    shmdt(iptr);
    shmctl(shm_id, IPC_RMID, (struct shmid_ds *) 0);

    return 0;
}