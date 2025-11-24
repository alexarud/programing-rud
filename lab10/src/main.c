#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "lib.h"

int main() {
    int n = 5;

    int** matrix = malloc((unsigned long) n * sizeof(int*));

    if(!matrix)
        return 1;

    for(int i = 0; i < n; i++){
        *(matrix + i) = malloc((unsigned long)n * sizeof(int));

        if(*(matrix+i)) continue;

        for(int j = 0; j < i; j++)
            free(*(matrix+j));

        free(matrix);
        return 1;
    }

    srand((unsigned) time(NULL));

    for(int i = 0; i < n; i++){
        int* row = *(matrix + i);
        for(int j = 0; j < n; j++){
            *(row + j) = rand() % 100;
        }
    }

    print_matrix(matrix, n);

    int* diagonal = malloc((unsigned long)n*sizeof(int));

    if(!diagonal){
        for(int j = 0; j < n; j++)
            free(*(matrix+j));

        free(matrix);
        return 1;
    }

    copy_main_diagonal(matrix, n, diagonal);
    sort_array(diagonal, n);
    print_array(diagonal, n);


    free(diagonal);

    for(int i = 0; i < n; i++){
        free(*(matrix + i));
    }

    free(matrix);

    return 0;
}
