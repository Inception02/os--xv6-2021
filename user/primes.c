#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int N = 35;
int pfd[2];
int fdr, fdw;
int first = 1;

int main(int argc, char* argv[])
{
    if (first) {
        first = 0;
        pipe(pfd);
        fdr = pfd[0];
        fdw = pfd[1];
        for (int i = 2; i <= N; ++i) {
            write(pfd[1], (char*)&i, 4);
        }
        close(fdw);
    }
    if (fork() == 0) {
        int p = 0;
        if (read(fdr, (char*)&p, 4)) {
            printf("prime %d\n", p);
            pipe(pfd);
            fdw = pfd[1];
        }
        int i;
        while (read(fdr, (char*)&i, 4)) {
            if (i % p != 0) {
                write(fdw, (char*)&i, 4);
            }
        }
        fdr = pfd[0];
        close(fdw);
        main(argc, argv);
    }
    else {
        wait((int*)0);
        close(pfd[0]);
    }

    exit(0);
}