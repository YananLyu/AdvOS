#include <sys/types.h>  // should be first, because dependency
#include <sys/ipc.h>    // so order is important
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define KEY 7890  /* assume unique known by other process, work together */
#define PERM 0600  /* permission ugo; only owner can rw */

int main() {

    int shmid;
    int i;
    int * iptr;  /* point to shared memory variable */

    if(fork() == 0) {  // child process

        shmid = shmget(KEY, sizeof(int), PERM | IPC_CREAT);
        iptr = (int*) shmat(shmid, NULL, 0);

        for(i=0; i<10; i++) {
            printf("Child %d\n", (*iptr)++);
            sleep(1);
        }

        exit(0);
    } else {  // parent process
    
        shmid = shmget(KEY, sizeof(int), PERM | IPC_CREAT);
        iptr = (int*) shmat(shmid, (char*) 0, 0);  /* shmat: attach the pointer to the shared memory */

        for(i=0; i<10; i++) {
            printf("Parent %d\n", (*iptr)++);
            sleep(1);
        }

        shmdt(iptr);  /* detach it */
        shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);  /* destroying a shared memory segment */
    }
}
