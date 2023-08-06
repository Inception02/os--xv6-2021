#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buff[128];

int main(int argc, char* argv[])
{
    int pfd1[2], pfd2[2];
    pipe(pfd1);     //父进程到子进程
    pipe(pfd2);     //子进程到父进程

    if (fork() == 0) {
        //子进程
        close(pfd2[1]);
        close(pfd1[0]);
        int pid = getpid();
        if (read(pfd2[0], buff, 1) == 1) {
            printf("%d: received ping\n", pid);
            write(pfd1[1], buff, 1);
        }
    }
    else {
        //父进程
        close(pfd1[1]);
        close(pfd2[0]);
        int pid = getpid();
        write(pfd2[1], buff, 1);
        if (read(pfd1[0], buff, 1) == 1) {
            printf("%d: received pong\n", pid);
        }
    }

    exit(0);
}