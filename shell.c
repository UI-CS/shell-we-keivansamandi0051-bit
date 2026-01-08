#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100
#define HISTORY 5

char hist[HISTORY][MAX];
int hcount = 0;

void add_history(char *cmd) {
    strcpy(hist[hcount % HISTORY], cmd);
    hcount++;
}

void show_history() {
    int start = hcount - HISTORY;
    if(start < 0) start = 0;

    for(int i = start; i < hcount; i++)
        printf("%d %s\n", i + 1, hist[i % HISTORY]);
}

int main() {
    char cmd[MAX];

    while(1) {
        printf("myshell> ");
        fflush(stdout);

        if(!fgets(cmd, MAX, stdin))
            break;

        cmd[strcspn(cmd, "\n")] = 0;

        if(strlen(cmd) == 0)
            continue;

        if(strcmp(cmd, "exit") == 0)
            break;

        if(strcmp(cmd, "history") == 0) {
            show_history();
            continue;
        }

        if(strcmp(cmd, "!!") == 0) {
            if(hcount == 0) {
                printf("No commands in history\n");
                continue;
            }
            strcpy(cmd, hist[(hcount - 1) % HISTORY]);
            printf("%s\n", cmd);
        }

        add_history(cmd);

        pid_t pid = fork();
        if(pid == 0) {
            execlp(cmd, cmd, NULL);
            printf("Command not found\n");
            return 0;
        }
        wait(NULL);
    }

    return 0;
}
