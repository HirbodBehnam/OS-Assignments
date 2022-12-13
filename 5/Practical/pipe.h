#ifndef PRACTICAL_PIPE_H
#define PRACTICAL_PIPE_H

#endif //PRACTICAL_PIPE_H

#include <sys/unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef TEST_PIPE_H
#define TEST_PIPE_H

#endif //TEST_PIPE_H

typedef struct {
    int reader;
    int writer;
} simplified_pipe;

simplified_pipe create_simple_pipe() {
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("cannot create pipe");
        exit(1);
    }
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