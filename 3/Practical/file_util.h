#ifndef PRACTICAL3_FILE_UTIL_H
#define PRACTICAL3_FILE_UTIL_H

#endif //PRACTICAL3_FILE_UTIL_H

#include <stdlib.h>

// Source filename to read from it
#define SOURCE_FILE_NAME "Practical_Q_1numbers.txt"

static int source_count_numbers(FILE *source);

/**
 * Reads all of the numbers in the file
 * @param count
 * @return
 */
long *source_read_all_numbers(int *count) {
    // Open the source file
    FILE *source = fopen(SOURCE_FILE_NAME, "r");
    if (source == NULL) {
        perror("cannot open source file");
        exit(1);
    }
    // Get the number of numbers in file
    *count = source_count_numbers(source);
    // Allocate memory for numbers
    long *numbers = malloc((*count) * sizeof(long));
    if (numbers == NULL) {
        perror("out of memory");
        exit(1);
    }
    // Read the file
    int index = 0;
    while (1) {
        long number;
        fscanf(source, "%ld", &number);
        if (feof(source))
            break;
        numbers[index] = number;
        index++;
    }
    // Done
    fclose(source);
    return numbers;
}

/**
 * Counts the numbers in a file
 * @param source The source file
 * @return The number of numbers in a file
 */
static int source_count_numbers(FILE *source) {
    rewind(source);
    int counter = 0;
    while (1) {
        int temp;
        fscanf(source, "%d", &temp);
        if (feof(source)) {
            rewind(source);
            return counter;
        }
        counter++;
    }
}