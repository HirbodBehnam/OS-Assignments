#ifndef PRACTICAL3_SINGLE_PROCESS_H
#define PRACTICAL3_SINGLE_PROCESS_H

#endif //PRACTICAL3_SINGLE_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include "external.h"

void single_process() {
    FILE *source = fopen("Practical_Q_1numbers.txt", "r");
    if (source == NULL) {
        perror("cannot open file");
        exit(1);
    }
    long accumulator;
    int a, b;
    fscanf(source, "%d %d", &a, &b);
    accumulator = fun(a, b);
    while (1) {
        fscanf(source, "%d %d", &a, &b);
        if (feof(source))
            break;
        accumulator = fun(accumulator, fun(a, b));
    }
    printf("%ld\n", accumulator);
}