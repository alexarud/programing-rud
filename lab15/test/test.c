#include "entity.h"
#include "list.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

/* --- vacancy_str_cmp --- */

START_TEST(test_str_cmp_equal)
{
    ck_assert_int_eq(vacancy_str_cmp("EPAM", "EPAM"), 0);
}
END_TEST

START_TEST(test_str_cmp_diff)
{
    ck_assert_int_lt(vacancy_str_cmp("EPAM", "SoftServe"), 0);
    ck_assert_int_gt(vacancy_str_cmp("SoftServe", "EPAM"), 0);
}
END_TEST

/* --- vacancy_english_rank --- */

START_TEST(test_english_rank)
{
    ck_assert_int_eq(vacancy_english_rank("pre-intermediate"), 0);
    ck_assert_int_eq(vacancy_english_rank("intermediate"), 1);
    ck_assert_int_eq(vacancy_english_rank("upper-intermediate"), 2);
    ck_assert_int_eq(vacancy_english_rank("advanced"), 3);
    ck_assert_int_eq(vacancy_english_rank("unknown"), -1);
}
END_TEST

/* --- list_init / list_free --- */

START_TEST(test_list_init)
{
    VacancyList list;
    list_init(&list);
    ck_assert_int_eq(list.size, 0);
    ck_assert_int_eq(list.capacity, 0);
    ck_assert_ptr_null(list.data);
    list_free(&list);
}
END_TEST

/* --- list_push --- */

static Vacancy make_vacancy(const char *company, int exp, int paid,
                             int sal_max, VacancyType type) {
    Vacancy v;
    v.type = type;
    v.paid_vacation = paid;
    v.years_exp = exp;
    v.salary.min = 1000;
    v.salary.max = sal_max;
    v.extra2 = 0;
    int i = 0;
    while (*(company + i) && i < MAX_STR - 1) {
        *(v.company + i) = *(company + i);
        i++;
    }
    *(v.company + i) = '\0';
    v.position[0] = '\0';
    v.english_level[0] = '\0';
    v.extra1[0] = '\0';
    return v;
}

START_TEST(test_list_push_single)
{
    VacancyList list;
    list_init(&list);
    Vacancy v = make_vacancy("EPAM", 3, 1, 5000, VACANCY_OFFICE);
    ck_assert_int_eq(list_push(&list, &v), 1);
    ck_assert_int_eq(list.size, 1);
    ck_assert_str_eq(list.data->company, "EPAM");
    list_free(&list);
}
END_TEST

START_TEST(test_list_push_grow)
{
    VacancyList list;
    list_init(&list);
    for (int i = 0; i < 10; i++) {
        Vacancy v = make_vacancy("Co", i, 1, 1000 * i, VACANCY_OFFICE);
        ck_assert_int_eq(list_push(&list, &v), 1);
    }
    ck_assert_int_eq(list.size, 10);
    list_free(&list);
}
END_TEST

/* --- list_remove --- */

START_TEST(test_list_remove_valid)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 2, 0, 2000, VACANCY_OFFICE);
    Vacancy c = make_vacancy("C", 3, 0, 3000, VACANCY_OFFICE);
    list_push(&list, &a);
    list_push(&list, &b);
    list_push(&list, &c);
    ck_assert_int_eq(list_remove(&list, 1), 1);
    ck_assert_int_eq(list.size, 2);
    ck_assert_str_eq((list.data + 1)->company, "C");
    list_free(&list);
}
END_TEST

START_TEST(test_list_remove_invalid)
{
    VacancyList list;
    list_init(&list);
    Vacancy v = make_vacancy("X", 1, 0, 1000, VACANCY_OFFICE);
    list_push(&list, &v);
    ck_assert_int_eq(list_remove(&list, 5), 0);
    ck_assert_int_eq(list_remove(&list, -1), 0);
    ck_assert_int_eq(list.size, 1);
    list_free(&list);
}
END_TEST

/* --- list_filter_exp_and_vacation --- */

START_TEST(test_filter_exp_vacation)
{
    VacancyList list;
    list_init(&list);
    Vacancy v1 = make_vacancy("A", 3, 1, 5000, VACANCY_OFFICE);
    Vacancy v2 = make_vacancy("B", 1, 1, 2000, VACANCY_OFFICE);
    Vacancy v3 = make_vacancy("C", 4, 0, 6000, VACANCY_REMOTE);
    Vacancy v4 = make_vacancy("D", 5, 1, 9000, VACANCY_REMOTE);
    list_push(&list, &v1);
    list_push(&list, &v2);
    list_push(&list, &v3);
    list_push(&list, &v4);

    VacancyList result;
    list_init(&result);
    list_filter_exp_and_vacation(&list, &result, 2, 1);
    ck_assert_int_eq(result.size, 2);
    list_free(&result);
    list_free(&list);
}
END_TEST

START_TEST(test_filter_empty)
{
    VacancyList list;
    list_init(&list);
    VacancyList result;
    list_init(&result);
    list_filter_exp_and_vacation(&list, &result, 2, 1);
    ck_assert_int_eq(result.size, 0);
    list_free(&result);
    list_free(&list);
}
END_TEST

/* --- list_sort_by_salary --- */

START_TEST(test_sort_by_salary)
{
    VacancyList list;
    list_init(&list);
    Vacancy v1 = make_vacancy("A", 1, 0, 3000, VACANCY_OFFICE);
    Vacancy v2 = make_vacancy("B", 1, 0, 9000, VACANCY_REMOTE);
    Vacancy v3 = make_vacancy("C", 1, 0, 5000, VACANCY_OFFICE);
    list_push(&list, &v1);
    list_push(&list, &v2);
    list_push(&list, &v3);
    list_sort_by_salary(&list);
    ck_assert_int_eq(list.data->salary.max, 9000);
    ck_assert_int_eq((list.data + 1)->salary.max, 5000);
    ck_assert_int_eq((list.data + 2)->salary.max, 3000);
    list_free(&list);
}
END_TEST

/* --- vacancy_read_file --- */

START_TEST(test_read_file)
{
    FILE *f = fopen("/tmp/test_lab15.txt", "w");
    fprintf(f, "office;yes;Junior Java Developer;EPAM;2;1000;2000;intermediate;Харків;yes\n");
    fprintf(f, "remote;yes;Senior Android Developer;EPAM;5;5000;10000;upper-intermediate;EST;yes\n");
    fclose(f);

    f = fopen("/tmp/test_lab15.txt", "r");
    Vacancy arr[8];
    int n = vacancy_read_file(f, arr, 8);
    fclose(f);

    ck_assert_int_eq(n, 2);
    ck_assert_str_eq(arr->company, "EPAM");
    ck_assert_int_eq(arr->years_exp, 2);
    ck_assert_int_eq((arr + 1)->salary.max, 10000);
}
END_TEST

/* --- Suite --- */

Suite *lab15_suite(void) {
    Suite *s = suite_create("Lab15");

    TCase *tc_entity = tcase_create("entity");
    tcase_add_test(tc_entity, test_str_cmp_equal);
    tcase_add_test(tc_entity, test_str_cmp_diff);
    tcase_add_test(tc_entity, test_english_rank);
    tcase_add_test(tc_entity, test_read_file);
    suite_add_tcase(s, tc_entity);

    TCase *tc_list = tcase_create("list");
    tcase_add_test(tc_list, test_list_init);
    tcase_add_test(tc_list, test_list_push_single);
    tcase_add_test(tc_list, test_list_push_grow);
    tcase_add_test(tc_list, test_list_remove_valid);
    tcase_add_test(tc_list, test_list_remove_invalid);
    tcase_add_test(tc_list, test_filter_exp_vacation);
    tcase_add_test(tc_list, test_filter_empty);
    tcase_add_test(tc_list, test_sort_by_salary);
    suite_add_tcase(s, tc_list);

    return s;
}

int main(void) {
    Suite *s = lab15_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed == 0 ? 0 : 1;
}
