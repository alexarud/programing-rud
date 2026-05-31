/**
 * @file lib.c
 * @brief Реалізація функцій трикутника Паскаля
 */
#include "lib.h"
#include <stdlib.h>

int pascal_recursive(int n, int k) {
    if (k == 0 || k == n)
        return 1;
    return pascal_recursive(n - 1, k - 1) + pascal_recursive(n - 1, k);
}

int **pascal_iterative(int n) {
    int **triangle = malloc((unsigned long)n * sizeof(int *));
    if (!triangle)
        return NULL;

    for (int i = 0; i < n; i++) {
        *(triangle + i) = malloc((unsigned long)(i + 1) * sizeof(int));
        if (!*(triangle + i)) {
            for (int j = 0; j < i; j++)
                free(*(triangle + j));
            free(triangle);
            return NULL;
        }
        *(*(triangle + i)) = 1;
        *(*(triangle + i) + i) = 1;
        for (int j = 1; j < i; j++) {
            *(*(triangle + i) + j) =
                *(*(triangle + i - 1) + j - 1) + *(*(triangle + i - 1) + j);
        }
    }
    return triangle;
}

void free_pascal(int **triangle, int n) {
    for (int i = 0; i < n; i++)
        free(*(triangle + i));
    free(triangle);
}
