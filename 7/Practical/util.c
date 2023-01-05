#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

static const char *prefix_cut(const char *str, const char *pre) {
    size_t pre_size = strlen(pre);
    if (strncmp(pre, str, pre_size) == 0) { // has prefix
        return str + pre_size;
    } else { // does not have prefix
        return NULL;
    }
}

struct ParsedArguments parse_arguments(int argc, char **argv) {
    // Check count
    if (argc < 3) {
        puts("please pass --num_threads={integer} and --num_iterations={integer} as arguments");
        exit(1);
    }
    // Parse them
    struct ParsedArguments result = {0, 0};
    for (int i = 1; i < argc; i++) {
        // Parse threads
        const char *threads_string = prefix_cut(argv[i], "--num_threads=");
        if (threads_string != NULL) {
            result.thread_count = atoi(threads_string);
            continue;
        }
        // Parse iterations
        const char *iterations_string = prefix_cut(argv[i], "--num_iterations=");
        if (iterations_string != NULL) {
            result.iteration_count = atoi(iterations_string);
            continue;
        }
    }
    return result;
}

char *get_random_string() {
    static atomic_int counter;
    char *buffer = malloc(10 * sizeof(char));
    int next = counter++; // this is atomic to be thread safe
    sprintf(buffer, "str%d", next);
    return buffer;
}

char *clone_string(const char *str) {
    char *clone = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(clone, str);
    return clone;
}