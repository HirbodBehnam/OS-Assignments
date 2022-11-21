#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "external.h"
#include "file_util.h"

/**
 * Number of child processes to spawn
 */
#define WORKER_COUNT 8
/**
 * How frequently we should empty the read buffer of slaves?
 */
#define BUFFER_EMPTY_INTERVAL 1000

/**
 * Defines a job to be send to a slave
 */
typedef struct {
    long a;
    long b;
} Job;

/**
 * The result of a job which comes from slave to master
 */
typedef struct {
    long result;
} JobResult;

/**
 * Waits for all of the children of process to exit.
 * From https://stackoverflow.com/a/23872806/4213397
 */
void wait_for_all_children() {
    int status;
    while (wait(&status) > 0);
}

void reduce_step(int numbers_size, long *numbers, int fd_write, int fd_read);

int main() {
    bool master = true; // detects if process is master or slave
    int master_to_slave_pipe[2], slave_to_master_pipe[2];
    if (pipe(master_to_slave_pipe) == -1) {
        perror("cannot create pipe");
        exit(1);
    }
    if (pipe(slave_to_master_pipe) == -1) {
        perror("cannot create pipe");
        exit(1);
    }
    // Fork and send data
    for (int i = 0; i < WORKER_COUNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("cannot fork");
            exit(1);
        }
        if (pid == 0) {       // don't fork anymore on childs
            close(master_to_slave_pipe[1]); // close unused write end
            close(slave_to_master_pipe[0]); // close unused read end
            master = false;
            break;
        }
    }
    // On master send the works
    if (master) {
        close(master_to_slave_pipe[0]); // close unused read end
        close(slave_to_master_pipe[1]); // close unused write end
        // Read numbers to start the reduction
        int numbers_size;
        long *numbers = source_read_all_numbers(&numbers_size);
        // Reduce until numbers_size is zero
        while (numbers_size != 1) {
            printf("executing with %d numbers\n", numbers_size);
            reduce_step(numbers_size, numbers, master_to_slave_pipe[1], slave_to_master_pipe[0]);
            // if it's even, do nothing, otherwise + 1
            numbers_size = numbers_size / 2 + (numbers_size % 2);
        }
        close(master_to_slave_pipe[1]); // close for childs
        close(slave_to_master_pipe[0]);
        printf("Result is %ld\n", numbers[0]);
        free(numbers);
        // Wait for children
        wait_for_all_children();
    } else { // On slaves wait for work
        Job job;
        while (read(master_to_slave_pipe[0], &job, sizeof(job)) > 0) {
            JobResult result = {fun(job.a, job.b)};
            write(slave_to_master_pipe[1], &result, sizeof(result));
        }
        // Cleanup
        close(master_to_slave_pipe[0]);
        close(slave_to_master_pipe[1]);
    }
    // Done
    return 0;
}

void reduce_step(const int numbers_size, long *numbers, const int fd_write, const int fd_read) {
    JobResult result;
    int read_results = 0;
    int expected_reads = numbers_size / 2;
    // Send all to children
    for (int i = 1; i < numbers_size; i += 2) {
        Job job = {numbers[i - 1], numbers[i]};
        //printf("wrote %d\n", (i - 1) / 2);
        write(fd_write, &job, sizeof(job)); // this is non-blocking until internal buffer is not full!
        if ((i - 1) / 2 % BUFFER_EMPTY_INTERVAL == BUFFER_EMPTY_INTERVAL - 1) { // slowly empty buffer
            for (int j = 0; j < BUFFER_EMPTY_INTERVAL && read_results < expected_reads; j++) {
                //printf("read %d\n", read_results);
                read(fd_read, &result, sizeof(result));
                numbers[read_results] = result.result;
                read_results++;
            }
        }
    }
    // Get all results
    printf("expecting %d jobs; already read %d results\n", expected_reads, read_results);
    for (; read_results < expected_reads; read_results++) {
        read(fd_read, &result, sizeof(result));
        numbers[read_results] = result.result;
    }
    // Update the last element if needed
    if (numbers_size % 2 == 1) {
        numbers[numbers_size / 2] = numbers[numbers_size - 1];
    }
}
