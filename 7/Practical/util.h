#ifndef PRACTICAL_UTIL_H
#define PRACTICAL_UTIL_H

#endif //PRACTICAL_UTIL_H

struct ParsedArguments {
    int thread_count;
    int iteration_count;
};

struct ParsedArguments parse_arguments(int argc, char **argv);

/**
 * Although there is a "random" in it's name, this just creates a sequential string
 * with a atomic counter to simulate distinct strings.
 * @return A random string
 */
char *get_random_string();

