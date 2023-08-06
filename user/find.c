#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find(char* path, char* fileName)
{
    char buff[512];
    char* p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) {
        if (strcmp(fmtname(path), fileName) == 0) {
            printf("%s\n", path);
        }
    }
    else if (st.type == T_DIR) {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buff)) {
            printf("Path too long\n");
        }
        else {
            strcpy(buff, path);
            p = buff + strlen(buff);
            *p = '/';
            p++;
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buff, &st) < 0) {
                    printf("cannot stat %s\n", buff);
                    continue;
                }

                if (strcmp(fmtname(buff), ".") != 0 && strcmp(fmtname(buff), "..") != 0) {
                    find(buff, fileName);
                }
            }
        }
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Syntax Error\nusage: find [path] [filename]\n");
        exit(0);
    }

    find(argv[1], argv[2]);

    exit(0);
}