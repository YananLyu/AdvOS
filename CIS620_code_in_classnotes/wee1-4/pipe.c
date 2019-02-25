#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

    int p[2];  // because pipes only require two for read and write.
    pid_t cpid;
    pipe(p); // use p's address. because p is array, p is the address.
    char *msg = "CIS620 is a great class";
    int MAX = 1024;
    char buf[MAX];

    // suppose we want child process send msg to parent by pipe.
    if((cpid = fork()) == 0) {
        close(p[0]);  // child don't need p[0]. even it is not required, it is clear.
        write(p[1], msg, strlen(msg)+1);  // p[1] is fp, file pointer;
        exit(0);
    }

    // parent process;
    close(p[1]);  // for parent process, it just read msg, so it don't use p[1] to write
    read(p[0], buf, MAX);  // read() is a blocking call.
    printf("msg form child is: %s\n", buf);

    return 0;
}