#include "lib.h"
#include <check.h>
#include <stdlib.h>

/**
 * Тестуємо isPrime на наборі типових значень.
 */
START_TEST(test_isPrime)
{
    #define DATA_SIZE 7

    int input_data[]      = { -1, 0, 1, 2, 3, 4, 17 };
    int expected_values[] = {  0, 0, 0, 1, 1, 0,  1 };

    for (int i = 0; i < DATA_SIZE; i++) {
        int actual_value = isPrime(input_data[i]);
        ck_assert_int_eq(expected_values[i], actual_value);
    }
}
END_TEST

/**
 * Тестуємо random_int: значення завжди в діапазоні [min; max].
 */
START_TEST(test_random_int_range)
{
    int min = 5;
    int max = 10;

    for (int i = 0; i < 1000; i++) {
        int value = random_int(min, max);
        ck_assert_int_ge(value, min);
        ck_assert_int_le(value, max);
    }
}
END_TEST

/**
 * Тестуємо countIncreasingPairs на кількох варіантах.
 */
START_TEST(test_countIncreasingPairs)
{
    int res1 = countIncreasingPairs(3, 1, 2, 3);             // strictly increasing
    ck_assert_int_eq(res1, 3); // пари: (1,2), (1,3), (2,3)

    int res2 = countIncreasingPairs(5, 5, 4, 3, 2, 1);       // strictly decreasing
    ck_assert_int_eq(res2, 0);

    int res3 = countIncreasingPairs(3, 1, 1, 1);             // all equal
    ck_assert_int_eq(res3, 0);
}
END_TEST

/**
 * Тестуємо squareMatrix на матриці 2x2.
 */
START_TEST(test_squareMatrix)
{
    int matrix[] = {
        1, 2,
        3, 4
    };

    int result[4];

    squareMatrix(matrix, 2, 2, result);

    int expected[] = {
        7, 10,
        15, 22
    };

    for (int i = 0; i < 4; i++) {
        ck_assert_int_eq(result[i], expected[i]);
    }
}
END_TEST

/**
 * Тестуємо get_determinant для 1x1, 2x2, 3x3 і неквадратної матриці.
 */
START_TEST(test_get_determinant)
{
    int m1[] = { 5 };
    ck_assert_int_eq(get_determinant(m1, 1, 1), 5);

    int m2[] = {
        1, 2,
        3, 4
    };
    ck_assert_int_eq(get_determinant(m2, 2, 2), -2);

    int m3[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    ck_assert_int_eq(get_determinant(m3, 3, 3), 1);

    int m_bad[] = {
        1, 2, 3,
        4, 5, 6
    };
    ck_assert_int_eq(get_determinant(m_bad, 2, 3), 0);

    int m_big[] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    ck_assert_int_eq(get_determinant(m_bad, 4, 4), 0);
}
END_TEST

/**
 * Викликаємо printMatrix просто для покриття коду.
 */
START_TEST(test_printMatrix)
{
    int matrix[] = {
        1, 2,
        3, 4
    };

    printMatrix(matrix, 2, 2);
}
END_TEST

int main(void)
{
    Suite *s = suite_create("Programming");
    TCase *tc_core = tcase_create("lab09");

    tcase_add_test(tc_core, test_isPrime);
    tcase_add_test(tc_core, test_random_int_range);
    tcase_add_test(tc_core, test_countIncreasingPairs);
    tcase_add_test(tc_core, test_squareMatrix);
    tcase_add_test(tc_core, test_get_determinant);
    tcase_add_test(tc_core, test_printMatrix);

    suite_add_tcase(s, tc_core);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);

    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


