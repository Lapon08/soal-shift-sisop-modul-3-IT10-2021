#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int link[2],link2[2];
    int saved_stdout;
    pipe(link);pipe(link2);

    if (fork() > 0) {
        exit(EXIT_SUCCESS);
    }

    if (fork() == 0) {
        dup2(link[1],STDOUT_FILENO);
        // close
        close(link[0]);close(link2[0]);close(link2[1]);
        execlp("/bin/ps","ps", "aux", NULL);
    }

    if (fork() == 0) {
        dup2(link[0],STDIN_FILENO);
        // close
        close(link[1]);close(link2[0]);
        dup2(link2[1],STDOUT_FILENO);
        execlp("/bin/sort","sort", "-nrk","3,3", NULL);
    }
    
    if (fork()== 0) {
        dup2(link2[0],STDIN_FILENO);
        // close
        close(link[1]);close(link[0]);close(link2[1]);
        execlp("/bin/head","head", "-5", NULL);
    }

}