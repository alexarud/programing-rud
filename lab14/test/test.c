#include "lib.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- str_cmp --- */

START_TEST(test_str_cmp_equal)
{
    ck_assert_int_eq(str_cmp("hello", "hello"), 0);
}
END_TEST

START_TEST(test_str_cmp_less)
{
    ck_assert_int_lt(str_cmp("EPAM", "SoftServe"), 0);
}
END_TEST

START_TEST(test_str_cmp_greater)
{
    ck_assert_int_gt(str_cmp("SoftServe", "EPAM"), 0);
}
END_TEST

/* --- english_level_rank --- */

START_TEST(test_english_rank_order)
{
    ck_assert_int_eq(english_level_rank("pre-intermediate"), 0);
    ck_assert_int_eq(english_level_rank("intermediate"), 1);
    ck_assert_int_eq(english_level_rank("upper-intermediate"), 2);
    ck_assert_int_eq(english_level_rank("advanced"), 3);
}
END_TEST

START_TEST(test_english_rank_unknown)
{
    ck_assert_int_eq(english_level_rank("beginner"), -1);
}
END_TEST

/* --- read_vacancies --- */

static VacancyCollection make_test_collection(void) {
    FILE *f = fopen("/tmp/test_lab14.txt", "w");
    fprintf(f, "office;yes;Junior Java Developer;EPAM;2;1000;2000;intermediate;Харків, Сумська, 1;yes\n");
    fprintf(f, "office;yes;Senior Android Developer;GlobalLogic;6;4000;7000;advanced;Київ, Хрещатик, 10;yes\n");
    fprintf(f, "remote;yes;Senior Android Developer;EPAM;5;5000;10000;upper-intermediate;EST;yes\n");
    fprintf(f, "remote;no;Project Manager;SoftServe;2;3000;6000;advanced;PT;no\n");
    fclose(f);

    f = fopen("/tmp/test_lab14.txt", "r");
    VacancyCollection col;
    read_vacancies(f, &col);
    fclose(f);
    return col;
}

START_TEST(test_read_vacancies_counts)
{
    VacancyCollection col = make_test_collection();
    ck_assert_int_eq(col.office_count, 2);
    ck_assert_int_eq(col.remote_count, 2);
}
END_TEST

START_TEST(test_read_vacancies_fields)
{
    VacancyCollection col = make_test_collection();
    ck_assert_str_eq(col.office->base.company, "EPAM");
    ck_assert_int_eq(col.office->base.years_exp, 2);
    ck_assert_int_eq(col.office->base.paid_vacation, 1);
    ck_assert_int_eq(col.office->base.salary.min, 1000);
    ck_assert_int_eq(col.office->base.salary.max, 2000);
    ck_assert_int_eq(col.office->has_gym, 1);
}
END_TEST

START_TEST(test_read_vacancies_remote_fields)
{
    VacancyCollection col = make_test_collection();
    ck_assert_str_eq(col.remote->timezone, "EST");
    ck_assert_int_eq(col.remote->remote_exp, 1);
}
END_TEST

/* --- filter_exp_and_vacation --- */

START_TEST(test_filter_exp_and_vacation)
{
    VacancyCollection col = make_test_collection();
    Vacancy result[16];
    int count = 0;
    filter_exp_and_vacation(&col, result, &count, 2, 1);
    /* office[1]: 6 р., yes — підходить; remote[0]: 5 р., yes — підходить */
    ck_assert_int_eq(count, 2);
}
END_TEST

START_TEST(test_filter_exp_and_vacation_empty)
{
    VacancyCollection col;
    col.office_count = 0;
    col.remote_count = 0;
    Vacancy result[4];
    int count = 0;
    filter_exp_and_vacation(&col, result, &count, 2, 1);
    ck_assert_int_eq(count, 0);
}
END_TEST

/* --- sort_and_find_intermediate --- */

START_TEST(test_sort_and_find_intermediate_found)
{
    VacancyCollection col = make_test_collection();
    Vacancy result;
    int found = sort_and_find_intermediate(&col, &result);
    ck_assert_int_eq(found, 1);
    ck_assert_int_ge(english_level_rank(result.english_level), 1);
}
END_TEST

START_TEST(test_sort_and_find_intermediate_empty)
{
    VacancyCollection col;
    col.office_count = 0;
    col.remote_count = 0;
    Vacancy result;
    ck_assert_int_eq(sort_and_find_intermediate(&col, &result), 0);
}
END_TEST

/* --- find_remote_max_salary --- */

START_TEST(test_find_remote_max_salary)
{
    VacancyCollection col = make_test_collection();
    RemoteVacancy result;
    int found = find_remote_max_salary(&col, &result);
    ck_assert_int_eq(found, 1);
    ck_assert_int_eq(result.base.salary.max, 10000);
}
END_TEST

START_TEST(test_find_remote_max_salary_empty)
{
    VacancyCollection col;
    col.remote_count = 0;
    RemoteVacancy result;
    ck_assert_int_eq(find_remote_max_salary(&col, &result), 0);
}
END_TEST

/* --- Suite --- */

Suite *lab14_suite(void) {
    Suite *s = suite_create("Lab14");

    TCase *tc_str = tcase_create("str_cmp");
    tcase_add_test(tc_str, test_str_cmp_equal);
    tcase_add_test(tc_str, test_str_cmp_less);
    tcase_add_test(tc_str, test_str_cmp_greater);
    suite_add_tcase(s, tc_str);

    TCase *tc_eng = tcase_create("english_level_rank");
    tcase_add_test(tc_eng, test_english_rank_order);
    tcase_add_test(tc_eng, test_english_rank_unknown);
    suite_add_tcase(s, tc_eng);

    TCase *tc_read = tcase_create("read_vacancies");
    tcase_add_test(tc_read, test_read_vacancies_counts);
    tcase_add_test(tc_read, test_read_vacancies_fields);
    tcase_add_test(tc_read, test_read_vacancies_remote_fields);
    suite_add_tcase(s, tc_read);

    TCase *tc_m1 = tcase_create("filter_exp_and_vacation");
    tcase_add_test(tc_m1, test_filter_exp_and_vacation);
    tcase_add_test(tc_m1, test_filter_exp_and_vacation_empty);
    suite_add_tcase(s, tc_m1);

    TCase *tc_m2 = tcase_create("sort_and_find_intermediate");
    tcase_add_test(tc_m2, test_sort_and_find_intermediate_found);
    tcase_add_test(tc_m2, test_sort_and_find_intermediate_empty);
    suite_add_tcase(s, tc_m2);

    TCase *tc_m3 = tcase_create("find_remote_max_salary");
    tcase_add_test(tc_m3, test_find_remote_max_salary);
    tcase_add_test(tc_m3, test_find_remote_max_salary_empty);
    suite_add_tcase(s, tc_m3);

    return s;
}

int main(void) {
    Suite *s = lab14_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed == 0 ? 0 : 1;
}
