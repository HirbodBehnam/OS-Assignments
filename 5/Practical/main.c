#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include "pipe.h"

#define WORKER_COUNT 4
#define MAX_QUEUED_JOBS 100

typedef struct {
    unsigned long long n;
    unsigned long long m;
} ackerman_job;

simplified_pipe master_to_dispatcher, dispatcher_to_slave;
int thread_busy[WORKER_COUNT];

unsigned long long ackerman(unsigned long long m, unsigned long long n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackerman(m - 1, 1);
    else
        return ackerman(m - 1, ackerman(m, n - 1));
}

void *worker_thread(void *args) {
    int thread_id = *(int *) args;
    free(args);
    ackerman_job job;
    while (read(dispatcher_to_slave.reader, &job, sizeof(job)) > 0) {
        printf("A(%llu, %llu) = %llu\n", job.m, job.n, ackerman(job.m, job.n));
        thread_busy[thread_id] = false;
    }
    return NULL;
}

/**
 * Extracts the most prioritised job from a job list and shifts everything forward
 * @param jobs List of jobs
 * @param length Length of jobs
 * @return Job with least priority
 */
ackerman_job extract_job(ackerman_job *jobs, int length) {
    int min_index;
    unsigned long long min_priority = ULLONG_MAX;
    // Find index of minimum
    for (int i = 0; i < length; i++) {
        if (jobs[i].m + jobs[i].n < min_priority) {
            min_index = i;
            min_priority = jobs[i].m + jobs[i].n;
        }
    }
    // Shift everything forward
    ackerman_job result = jobs[min_index];
    for (int i = min_index + 1; i < length; i++)
        jobs[i - 1] = jobs[i];
    return result;
}

void *dispatcher_thread(void *args) {
    ackerman_job queued_jobs[MAX_QUEUED_JOBS];
    int queued_jobs_length = 0;
    while (1) {
        // Check if we have any new jobs
        ackerman_job new_job;
        ssize_t read_result;
        if ((read_result = read(master_to_dispatcher.reader, &new_job, sizeof(new_job))) > 0) {
            queued_jobs[queued_jobs_length] = new_job;
            queued_jobs_length++;
        } else {
            if (errno != EAGAIN) { // This error message is sent if there was nothing in the queue
                break;
            }
        }
        // Now see if we have any free thread
        for (int i = 0; i < WORKER_COUNT; i++) {
            if (!thread_busy[i] && queued_jobs_length != 0) {
                ackerman_job to_dispatch_job = extract_job(queued_jobs, queued_jobs_length);
                queued_jobs_length--;
                thread_busy[i] = true;
                write(dispatcher_to_slave.writer, &to_dispatch_job, sizeof(to_dispatch_job));
            }
        }
        // Don't fuckup the CPU
        usleep(50 * 1000);
    }
    simplified_pipe_close(&dispatcher_to_slave);
    return NULL;
}


int main() {
    // Setup pipes
    master_to_dispatcher = create_simple_pipe(true);
    dispatcher_to_slave = create_simple_pipe(false);
    // Create threads
    pthread_t threads[WORKER_COUNT];
    pthread_t dispatcher_id;
    pthread_create(&dispatcher_id, NULL, dispatcher_thread, NULL);
    for (int i = 0; i < WORKER_COUNT; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, worker_thread, id);
    }
    // Read data and assign to threads
    puts("Enter two numbers seperated by space (m and n). Enter (-1, -1) to abort app. You can enter as many numbers as you want.");
    while (1) {
        ackerman_job job;
        scanf("%lld %lld", &job.m, &job.n);
        if (job.m == -1 && job.n == -1)
            break;
        write(master_to_dispatcher.writer, &job, sizeof(ackerman_job));
    }
    // Wait for threads
    simplified_pipe_close(&master_to_dispatcher);
    pthread_join(dispatcher_id, NULL);
    for (int i = 0; i < WORKER_COUNT; i++)
        pthread_join(threads[i], NULL);
    return 0;
}
