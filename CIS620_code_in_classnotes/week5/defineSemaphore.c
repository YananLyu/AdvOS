#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

union semun {  /* there is no err in linux */
    int val;
    struct semid_ds *buf;
    ushort *array;
};

#define SEMKEY 9876
int semkey;

void P(int semaphore) {  /* lock/wait */

    struct sembuf psembuf;

    psembuf.sem_op = -1;  /* semaphore operation */
    psembuf.sem_flg = 0;
    psembuf.sem_num = semaphore;
    semop(semkey, &psembuf, 1);

    return;
}

void V(int semaphore) {  /* unlock/post */

    struct sembuf vsembuf;

    vsembuf.sem_op = 1;  /* semaphore operation */
    vsembuf.sem_flg = 0;
    vsembuf.sem_num = semaphore;
    semop(semkey, &vsembuf, 1);

    return;
}

void child() {

    int pid = getpid();
    printf("Child process %d is starting up...\n", pid);
    fflush(stdout);
    
    P(0);
    printf("Child process %d has cleared the semaphore!\n", pid);
    fflush(stdout);
    
    exit(0);
}

int main() {

    int pid1, pid2;
    short seminit[1];  /* when call the name, it is the address.
                          that's why we use array, even only one element */
    union semun semctlarg;

    semkey=semget(SEMKEY, 1, 0777|IPC_CREAT);  // LINE 14
    seminit[0] = 0;
    semctlarg.array = seminit;  /* there is no err in linux */
    semctl(semkey, 1, SETALL, semctlarg);

    if((pid1=fork()) == 0)
        child();
    if((pid2=fork()) == 0)
        child();
    
    printf("Child processes are %d and %d.\n", pid1, pid2);
    printf("Parent process is now waiting a while.\n");
    fflush(stdout);
    sleep(3);
    printf("Parent is now signalling the semaphore...\n");

    fflush(stdout);
    V(0);
    sleep(3);
    printf("Parent is now signalling the semaphore...\n");
    fflush(stdout);
    V(0);

    printf("Parent is now waiting for the children to exit.\n");
    fflush(stdout);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("Children have exited. Cleaning up...\n");
    semctl(semkey, 1, IPC_CREAT);

    return 0;
}
