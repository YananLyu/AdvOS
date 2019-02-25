//*********  ls -l | grep Jan | result  **********
//                 | : stdout 1
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

int main() {
    
    pid_t cpid;
    int status;
    int p[2];
    int pp[2];
    char buf[2048];
    char buff[2048];
    pipe(p);
    char *c = " -l";

    if((cpid = fork()) == 0) {  // child process
        close(p[0]);        
        dup2(p[1], 1);  // change stdout 1 to p[1]. go to the same descripter now
        execv("/bin/ls", &c);  // abnormally, it return. normally, it won't return.
        // exit(0);

        pid_t cpid2;
        if((cpid2 = fork()) == 0) {  // new child process. child process's child
            close(pp[1]);
            dup2(pp[0], 0);
            char *g = " test";
            execv("/bin/grep", &g);
        }
        exit(0);

    }

    // parent
    wait(&status);
    close(p[1]);
    read(p[0], buf, 2048);
    printf("msg from child1 is:\n%s\n", buf);

    read(pp[0], buff, 2048);
    printf("msg from child1 is:\n%s\n", buff);

    return 0;
}