#include "lib.h"
#include <check.h>
#include <stdlib.h>

/**
 * Тестуємо copy_main_diagonal на типових значеннях.
 */
START_TEST(test_copy_main_diagonal)
{
    const int N = 3;

    // Вихідні значення матриці (плоский масив для зручності)
    const int values[] = {
        5, 1, 9,
        4, 2, 8,
        7, 3, 6
    };

    // Виділяємо пам'ять під int** матрицю
    int **matrix = malloc((size_t)N * sizeof(int *));
    ck_assert_ptr_nonnull(matrix);

    for (int i = 0; i < N; i++) {
        matrix[i] = malloc((size_t)N * sizeof(int));
        ck_assert_ptr_nonnull(matrix[i]);
    }

    // Копіюємо значення з values у matrix[i][j]
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = values[i * N + j];
        }
    }

    int diag[3];

    copy_main_diagonal(matrix, N, diag);

    // Очікувана головна діагональ: 5, 2, 6
    ck_assert_int_eq(diag[0], 5);
    ck_assert_int_eq(diag[1], 2);
    ck_assert_int_eq(diag[2], 6);

    // Прибираємо за собою
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
END_TEST

/**
 * Тестуємо sort_array: чи сортується масив за зростанням.
 */
START_TEST(test_sort_array)
{
    int arr[] = { 9, 1, 5, 2, 7 };
    const int expected[] = { 1, 2, 5, 7, 9 };
    const size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_array(arr, (int)n);

    for (size_t i = 0; i < n; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

/**
 * Інтеграційний тест: copy_main_diagonal + sort_array разом.
 */
START_TEST(test_integration)
{
    const int N = 3;

    const int values[] = {
        5, 1, 9,
        4, 2, 8,
        7, 3, 6
    };

    int **matrix = malloc((size_t)N * sizeof(int *));
    ck_assert_ptr_nonnull(matrix);

    for (int i = 0; i < N; i++) {
        matrix[i] = malloc((size_t)N * sizeof(int));
        ck_assert_ptr_nonnull(matrix[i]);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = values[i * N + j];
        }
    }

    int diag[3];

    copy_main_diagonal(matrix, N, diag);
    sort_array(diag, N);

    // Було: 5, 2, 6 → стало: 2, 5, 6
    ck_assert_int_eq(diag[0], 2);
    ck_assert_int_eq(diag[1], 5);
    ck_assert_int_eq(diag[2], 6);

    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
END_TEST

/**
 * Викликаємо print_matrix та print_array просто для покриття коду.
 * (Перевіряємо, що не падають.)
 */
START_TEST(test_print_functions)
{
    const int n = 2;

    // 2x2 матриця
    int **matrix = malloc((size_t)n * sizeof(int *));
    ck_assert_ptr_nonnull(matrix);

    for (int i = 0; i < n; i++) {
        matrix[i] = malloc((size_t)n * sizeof(int));
        ck_assert_ptr_nonnull(matrix[i]);
    }

    // Значення:
    // 1 2
    // 3 4
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;

    int arr[] = { 10, 20 };

    print_matrix(matrix, n);
    print_array(arr, n);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
END_TEST

int main(void)
{
    Suite *s = suite_create("Programming");
    TCase *tc_core = tcase_create("lab_diag_sort");

    tcase_add_test(tc_core, test_copy_main_diagonal);
    tcase_add_test(tc_core, test_sort_array);
    tcase_add_test(tc_core, test_integration);
    tcase_add_test(tc_core, test_print_functions);

    suite_add_tcase(s, tc_core);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);

    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
