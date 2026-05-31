#include "lib.h"
#include <check.h>
#include <stdlib.h>

/* --- str_len --- */

START_TEST(test_str_len_empty)
{
    ck_assert_int_eq(str_len(""), 0);
}
END_TEST

START_TEST(test_str_len_basic)
{
    ck_assert_int_eq(str_len("hello"), 5);
    ck_assert_int_eq(str_len("a"), 1);
}
END_TEST

/* --- char_len --- */

START_TEST(test_char_len_ascii)
{
    ck_assert_int_eq(char_len((unsigned char)'A'), 1);
    ck_assert_int_eq(char_len((unsigned char)' '), 1);
}
END_TEST

START_TEST(test_char_len_cyrillic)
{
    ck_assert_int_eq(char_len(0xD0), 2);
    ck_assert_int_eq(char_len(0xD1), 2);
}
END_TEST

/* --- char_count --- */

START_TEST(test_char_count_ascii)
{
    ck_assert_int_eq(char_count("abc", 3), 3);
}
END_TEST

START_TEST(test_char_count_cyrillic)
{
    const char *s = "АБ";
    int bytes = str_len(s);
    ck_assert_int_eq(bytes, 4);
    ck_assert_int_eq(char_count(s, bytes), 2);
}
END_TEST

/* --- build_freq_table --- */

START_TEST(test_freq_empty)
{
    FreqEntry entries[4];
    int total = 0;
    int unique = build_freq_table("", 0, entries, &total);
    ck_assert_int_eq(unique, 0);
    ck_assert_int_eq(total, 0);
}
END_TEST

START_TEST(test_freq_ascii_equal)
{
    FreqEntry entries[8];
    int total = 0;
    int unique = build_freq_table("ab", 2, entries, &total);
    ck_assert_int_eq(unique, 2);
    ck_assert_int_eq(total, 2);
    int found_a = 0, found_b = 0;
    for (int i = 0; i < unique; i++) {
        FreqEntry *e = entries + i;
        if (e->byte_len == 1 && *(e->bytes) == 'a') {
            ck_assert_double_eq_tol(e->freq, 0.5, 1e-9);
            found_a = 1;
        }
        if (e->byte_len == 1 && *(e->bytes) == 'b') {
            ck_assert_double_eq_tol(e->freq, 0.5, 1e-9);
            found_b = 1;
        }
    }
    ck_assert_int_eq(found_a, 1);
    ck_assert_int_eq(found_b, 1);
}
END_TEST

START_TEST(test_freq_sum_one)
{
    FreqEntry entries[16];
    int total = 0;
    int unique = build_freq_table("hello", str_len("hello"), entries, &total);
    double sum = 0.0;
    for (int i = 0; i < unique; i++)
        sum += (entries + i)->freq;
    ck_assert_double_eq_tol(sum, 1.0, 1e-9);
}
END_TEST

START_TEST(test_freq_cyrillic_unique)
{
    const char *text = "аа";
    FreqEntry entries[8];
    int total = 0;
    int unique = build_freq_table(text, str_len(text), entries, &total);
    ck_assert_int_eq(unique, 1);
    ck_assert_int_eq(total, 2);
    ck_assert_double_eq_tol((entries + 0)->freq, 1.0, 1e-9);
}
END_TEST

START_TEST(test_freq_mixed)
{
    const char *text = "аb";
    FreqEntry entries[8];
    int total = 0;
    int unique = build_freq_table(text, str_len(text), entries, &total);
    ck_assert_int_eq(unique, 2);
    ck_assert_int_eq(total, 2);
}
END_TEST

/* --- sort_freq_entries --- */

START_TEST(test_sort_freq_descending)
{
    FreqEntry entries[8];
    int total = 0;
    int unique = build_freq_table("aab", str_len("aab"), entries, &total);
    sort_freq_entries(entries, unique);
    ck_assert((entries + 0)->freq > (entries + 1)->freq);
    ck_assert_int_eq((int)*(entries + 0)->bytes, 'a');
}
END_TEST

int main(void)
{
    Suite *s = suite_create("FreqTable");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_str_len_empty);
    tcase_add_test(tc, test_str_len_basic);
    tcase_add_test(tc, test_char_len_ascii);
    tcase_add_test(tc, test_char_len_cyrillic);
    tcase_add_test(tc, test_char_count_ascii);
    tcase_add_test(tc, test_char_count_cyrillic);
    tcase_add_test(tc, test_freq_empty);
    tcase_add_test(tc, test_freq_ascii_equal);
    tcase_add_test(tc, test_freq_sum_one);
    tcase_add_test(tc, test_freq_cyrillic_unique);
    tcase_add_test(tc, test_freq_mixed);
    tcase_add_test(tc, test_sort_freq_descending);

    suite_add_tcase(s, tc);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
