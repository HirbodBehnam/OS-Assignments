#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

GraphMatrix *matrixes;
pthread_barrier_t barrier;

void *threadFunction(void *vargp) {
    // Load the index of this thread
    MatrixIndex index = *(MatrixIndex *) vargp;
    free(vargp);
    if (index.i == index.j)
        return NULL;
    // Calculate each depth
    for (int i = 1; i < matrixes[0].n; i++) {
        int result = matrixes[i - 1].matrix[index.i][index.j];
        for (int k = 0; k < matrixes[0].n; k++) {
            result = element_min(result,
                                 element_add(
                                         matrixes[i - 1].matrix[index.i][k],
                                         matrixes[0].matrix[k][index.j]
                                 )
            );
        }
        matrixes[i].matrix[index.i][index.j] = result;
        pthread_barrier_wait(&barrier);
    }
    // Done
    return NULL;
}

int main(int argc, char **argv) {
    // Get arguments
    if (argc < 3) {
        puts("Please enter the file name of matrix and 'n' which is the dimension of matrix");
        exit(1);
    }
    int n = atoi(argv[2]);
    if (n == 0) {
        puts("n cannot be zero");
        exit(1);
    }
    matrixes = malloc(n * sizeof(GraphMatrix));
    for (int i = 0; i < n; i++)
        matrixes[i] = new_matrix(n);
    // Read matrix
    read_matrix(argv[1], &matrixes[0]);
    // Create threads
    pthread_t threads[n][n];
    pthread_barrier_init(&barrier, NULL, n * n - n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            MatrixIndex *index = malloc(sizeof(MatrixIndex));
            index->i = i;
            index->j = j;
            pthread_create(&threads[i][j], NULL, threadFunction, index);
        }
    // Wait for them
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            pthread_join(threads[i][j], NULL);
    pthread_barrier_destroy(&barrier);
    // Print result
    puts("Input:");
    print_matrix(&matrixes[0]);
    puts("Output:");
    print_matrix(&matrixes[n - 1]);
    return 0;
}
