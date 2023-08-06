#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char args[MAXARG][512];
char *passArgs[MAXARG];
int preArgNum, argNum;
char ch;
char argBuff[512];


int readline()
{
    argNum = preArgNum;
    memset(argBuff, 0, sizeof(argBuff));

    int n;
    while(1) {
        n = read(0, &ch, sizeof(ch));
        if (n == 0) {
            return 0;
        }
        else if (n < 0) {
            fprintf(2, "Read Error\n");
            exit(1);
        }
        else {
            if (ch == '\n') {
                memcpy(args[argNum], argBuff, strlen(argBuff) + 1);
                argNum++;
                return 1;
            }
            else if (ch == ' ') {
                memcpy(args[argNum], argBuff, strlen(argBuff) + 1);
                argNum++;
                memset(argBuff, 0, sizeof(argBuff));
            }
            else {
                argBuff[strlen(argBuff)] = ch;
            }
        }
    }
}



int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Syntax Error\nusage: xargs [command] [arg1] [arg2] ... [argn]\n");
        exit(0);
    }

    preArgNum = argc - 1;
    for (int i = 0; i < preArgNum; ++i) {
        memcpy(args[i], argv[i + 1], strlen(argv[i + 1]));
    }
    while (readline()) {
        if (fork() == 0) {
            *args[argNum] = 0;
            int i = 0;
            while (*args[i]) {
                passArgs[i] = (char*)&args[i];
                ++i;
            }
            *passArgs[argNum] = 0;
            exec(passArgs[0], passArgs);
            printf("Exec Error\n");
            exit(0);
        }
        else {
            wait((int*)0);
        }
    }


    exit(0);
}