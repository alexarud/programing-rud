#include "lib.h"
#include <stdio.h>

// Копіює головну діагональ
void copy_main_diagonal(int **matrix, int n, int *diag) {
    for (int i = 0; i < n; i++) {
        *(diag + i) = *(*(matrix + i) + i);
    }
}

// Сортує масив
void sort_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (*(arr + j) < *(arr + i)) {
                int temp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = temp;
            }
        }
    }
}

// Друкує матрицю
void print_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        int* row = *(matrix + i);
        for (int j = 0; j < n; j++) {
            printf("%d\t", *(row + j));
        }
        printf("\n");
    }
}

// Друкує масив
void print_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");
}
