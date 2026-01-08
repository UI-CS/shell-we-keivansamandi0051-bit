#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int samples = 1000000;
    int pipes[2];
    pipe(pipes);

    pid_t pid = fork();

    if(pid==0) {
        close(pipes[0]);
        int inside = 0;
        srand(time(NULL)^getpid());

        for(int i=0;i<samples;i++){
            double x = (double)rand()/RAND_MAX;
            double y = (double)rand()/RAND_MAX;
            if(x*x + y*y <= 1) inside++;
        }

        write(pipes[1],&inside,sizeof(int));
        close(pipes[1]);
        exit(0);
    }

    close(pipes[1]);
    int count;
    read(pipes[0],&count,sizeof(int));
    wait(NULL);

    double pi = 4.0 * count / samples;
    printf("Estimated Pi = %f\n",pi);

    return 0;
}
