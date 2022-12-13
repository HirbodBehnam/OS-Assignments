#ifndef PRACTICAL_PIPE_H
#define PRACTICAL_PIPE_H

#endif //PRACTICAL_PIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {
    int reader;
    int writer;
} simplified_pipe;

simplified_pipe create_simple_pipe(bool non_blocking_read) {
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("cannot create pipe");
        exit(1);
    }
    // https://stackoverflow.com/a/36674490/4213397
    if (non_blocking_read)
        fcntl(pipe_fd[0], F_SETFL, fcntl(pipe_fd[0], F_GETFL) | O_NONBLOCK);
    simplified_pipe result = {
            .reader = pipe_fd[0],
            .writer = pipe_fd[1],
    };
    return result;
}

/**
 * Closes a pipe
 * @param pipe The pipe to close
 */
void simplified_pipe_close(simplified_pipe *pipe) {
    close(pipe->reader);
    close(pipe->writer);
}