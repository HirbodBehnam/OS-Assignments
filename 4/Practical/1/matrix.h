#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY (-1)

typedef struct {
    int n;
    int **matrix;
} GraphMatrix;

typedef struct {
    int i;
    int j;
} MatrixIndex;

/**
 * @brief Create a new matrix with given size
 *
 * @param n The size of matrix
 * @return GraphMatrix
 */
GraphMatrix new_matrix(int n) {
    GraphMatrix result = {.n = n};
    result.matrix = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        result.matrix[i] = (int *) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            result.matrix[i][j] = INFINITY;
    }
    return result;
}

/**
 * @brief Reads the matrix from a file
 *
 * @param path The path of the matrix to read from
 * @param matrix The matrix to save the results in
 */
void read_matrix(const char *path, GraphMatrix *matrix) {
    // Open the file
    FILE *source = fopen(path, "r");
    if (source == NULL) {
        perror("cannot open source file");
        exit(1);
    }
    // Read each element
    char buffer[10];
    for (int i = 0; i < matrix->n * matrix->n; i++) {
        fscanf(source, "%s", buffer);
        int x = i / matrix->n;
        int y = i % matrix->n;
        if ((unsigned char) buffer[0] == 236 || strcmp(buffer, "∞") == 0 || strcmp(buffer, "i") == 0) {
            matrix->matrix[x][y] = INFINITY;
        } else {
            matrix->matrix[x][y] = atoi(buffer);
        }
    }
    // Done
    fclose(source);
}

/**
 * Prints a matrix to stdout
 * @param matrix The matrix to print
 */
void print_matrix(GraphMatrix *matrix) {
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            if (matrix->matrix[i][j] == INFINITY)
                printf("∞ ");
            else
                printf("%d ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * Add to matrix cells. Handles INFINITY as well
 * @param a
 * @param b
 * @return a + b
 */
int element_add(int a, int b) {
    if (a == INFINITY || b == INFINITY)
        return INFINITY;
    return a + b;
}

/**
 * Minimum of two values respecting INFINITY
 * @param a
 * @param b
 * @return min(a, b)
 */
int element_min(int a, int b) {
    if (a == INFINITY && b == INFINITY)
        return INFINITY;
    if (a == INFINITY)
        return b;
    if (b == INFINITY)
        return a;
    return a > b ? b : a;
}