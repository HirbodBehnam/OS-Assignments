#include <stdio.h>
#include <pthread.h>
#include "pipe.h"

#define WORKER_COUNT 4
#define MAX_QUEUED_JOBS 100

typedef struct {
    unsigned long long n;
    unsigned long long m;
} ackerman_job;

simplified_pipe master_to_slave, slave_to_master;

unsigned long long ackerman(unsigned long long m, unsigned long long n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackerman(m - 1, 1);
    else
        return ackerman(m - 1, ackerman(m, n - 1));
}

void *worker_thread(void *args) {
    ackerman_job job;
    while (read(master_to_slave.reader, &job, sizeof(job)) > 0) {
        printf("A(%llu, %llu) = %llu\n", job.m, job.n, ackerman(job.m, job.n));
        char c;
        write(slave_to_master.writer, &c, sizeof(c));
    }
    return NULL;
}


int main() {
    // Setup pipes
    master_to_slave = create_simple_pipe();
    slave_to_master = create_simple_pipe();
    // Create threads
    pthread_t threads[WORKER_COUNT];
    for (int i = 0; i < WORKER_COUNT; i++)
        pthread_create(&threads[i], NULL, worker_thread, NULL);
    // Read data and assign to threads
    puts("Enter (-1, -1) to abort app");
    ackerman_job queued_jobs[MAX_QUEUED_JOBS];
    int queued_jobs_count = 0, busy_threads = 0;
    while (1) {
        ackerman_job job;
        printf("Enter two numbers seperated by space (m and n): ");
        scanf("%lld %lld", &job.m, &job.n);
        if (job.m == -1 && job.n == -1)
            break;
        // Check if all threads are busy
        if (busy_threads == WORKER_COUNT) {

        } else {
            busy_threads++;

        }
    }
    // Wait for jobs to be done
    while (busy_threads > 0) {
        busy_threads--;
        char c;
        read(slave_to_master.reader, &c, sizeof(c));
    }
    // Wait for threads
    for (int i = 0; i < WORKER_COUNT; i++)
        pthread_join(threads[i], NULL);
    return 0;
}
