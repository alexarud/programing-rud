#include "lib.h"
#include <check.h>
#include <stdlib.h>

/* --- pascal_recursive --- */

START_TEST(test_recursive_base_cases)
{
    ck_assert_int_eq(pascal_recursive(0, 0), 1);
    ck_assert_int_eq(pascal_recursive(5, 0), 1);
    ck_assert_int_eq(pascal_recursive(5, 5), 1);
}
END_TEST

START_TEST(test_recursive_known_values)
{
    ck_assert_int_eq(pascal_recursive(4, 2), 6);
    ck_assert_int_eq(pascal_recursive(6, 3), 20);
    ck_assert_int_eq(pascal_recursive(3, 1), 3);
    ck_assert_int_eq(pascal_recursive(3, 2), 3);
}
END_TEST

/* --- pascal_iterative --- */

START_TEST(test_iterative_row0)
{
    int **t = pascal_iterative(1);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(*(*(t + 0) + 0), 1);
    free_pascal(t, 1);
}
END_TEST

START_TEST(test_iterative_row5)
{
    const int expected[5][5] = {
        {1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 2, 1, 0, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 6, 4, 1},
    };

    int **t = pascal_iterative(5);
    ck_assert_ptr_nonnull(t);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j <= i; j++) {
            ck_assert_int_eq(*(*(t + i) + j), expected[i][j]);
        }
    }

    free_pascal(t, 5);
}
END_TEST

START_TEST(test_iterative_matches_recursive)
{
    const int n = 8;
    int **t = pascal_iterative(n);
    ck_assert_ptr_nonnull(t);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            ck_assert_int_eq(*(*(t + i) + j), pascal_recursive(i, j));
        }
    }

    free_pascal(t, n);
}
END_TEST

/* --- free_pascal smoke test --- */

START_TEST(test_free_pascal_no_crash)
{
    int **t = pascal_iterative(4);
    ck_assert_ptr_nonnull(t);
    free_pascal(t, 4);
}
END_TEST

/* --- test runner --- */

int main(void)
{
    Suite *s = suite_create("PascalTriangle");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_recursive_base_cases);
    tcase_add_test(tc, test_recursive_known_values);
    tcase_add_test(tc, test_iterative_row0);
    tcase_add_test(tc, test_iterative_row5);
    tcase_add_test(tc, test_iterative_matches_recursive);
    tcase_add_test(tc, test_free_pascal_no_crash);

    suite_add_tcase(s, tc);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
