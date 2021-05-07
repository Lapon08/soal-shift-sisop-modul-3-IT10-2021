#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

int main(){
    pid_t pid;
    pid = fork();
    int link[2],link2[2];
    int saved_stdout;
    pipe(link);pipe(link2);
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    pid_t child_id;
    child_id = fork();
    if (child_id == 0) {
        dup2(link[1],STDOUT_FILENO);
        // close
        close(link[0]);close(link2[0]);close(link2[1]);
        char *argvChild[] = {"ps", "aux", NULL};
        execv("/bin/ps", argvChild);
    }
    // while(wait(NULL)>0);

    child_id = fork();
    if (child_id == 0) {
        dup2(link[0],STDIN_FILENO);
        // close
        close(link[1]);close(link2[0]);
        dup2(link2[1],STDOUT_FILENO);
        char *argvChild[] = {"sort", "-nrk","3,3", NULL};
        execv("/bin/sort", argvChild);
    }
    
    // while(wait(NULL) >0);
    child_id = fork();
    if (child_id == 0) {
        dup2(link2[0],STDIN_FILENO);
        // close
        close(link[1]);close(link[0]);close(link2[1]);
        char *argvChild[] = {"head", "-5", NULL};
        execv("/bin/head", argvChild);
    }

}