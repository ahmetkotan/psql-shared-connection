
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <wait.h>
#include <math.h>
#include "pgman.h"


struct timeval
time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    struct timeval result = {diff / 1000000, (long int) diff % 1000000};
    return result;
}


int main() {
    pg_initialize();

    int child_count = 10, query_count = 200;

    fprintf(stdout, "Start with %d sub process and %d query each one.\n", child_count, query_count);
    struct timeval now, end;
    gettimeofday (&now, NULL);

    pid_t base_pid = getpid();

    for (int p = 0; p < child_count; p++) {
        pid_t pid = fork();
        if (pid == (pid_t) 0) {
            break;
        }
    }

    char *ip = (char *) malloc(20);
    sprintf(ip, "127.0.0.%d", getpid());

    for (int i = 0; i < query_count; i++) {
        char *tip = (char *) malloc(20);
        sprintf(tip, "%s-%d", ip, i);
        add_data(tip, "ahmetkotan");
    }
    fprintf(stdout, "Finished add_data() PID: %d\n", getpid());
    for (int j = 0; j < query_count; j++) {
        char *tip = (char *) malloc(20);
        sprintf(tip, "%s-%d", ip, j);
        change_data(tip, (float) j / 5);
    }
    fprintf(stdout, "Finished change_data() PID: %d\n", getpid());
    if (getpid() != base_pid)
        exit(0);

    if (getpid() == base_pid) {
        for (int w = 0; w < child_count; w++) {
            pid_t pid = wait(NULL);
            fprintf(stdout, "Finish PID: %d\n", pid);
        }
        gettimeofday (&end, NULL);
        fprintf(stdout, "End.\n");

        struct timeval diff = time_diff(now, end);
        int minute, second, millisecond;
        minute = diff.tv_sec / 60;
        second = diff.tv_sec % 60;
        millisecond = floor (diff.tv_usec / 1000);

        fprintf(stdout, "Elapsed Time: %dm %ds %dms\n", minute, second, millisecond);
    }

    pg_connection_close();
    return 0;
}
