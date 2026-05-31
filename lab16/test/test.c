/**
 * @file test.c
 * @brief Модульні тести для лабораторної роботи №16
 */
#include "entity.h"
#include "list.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

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
    *(v.position) = '\0';
    *(v.english_level) = '\0';
    *(v.extra1) = '\0';
    return v;
}

/* --- list_init --- */

START_TEST(test_list_init)
{
    VacancyList list;
    list_init(&list);
    ck_assert_int_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);
    list_free(&list);
}
END_TEST

/* --- list_push_back --- */

START_TEST(test_push_back_single)
{
    VacancyList list;
    list_init(&list);
    Vacancy v = make_vacancy("EPAM", 3, 1, 5000, VACANCY_OFFICE);
    ck_assert_int_eq(list_push_back(&list, &v), 1);
    ck_assert_int_eq(list.size, 1);
    ck_assert_str_eq(list.head->data.company, "EPAM");
    ck_assert_ptr_eq(list.head, list.tail);
    list_free(&list);
}
END_TEST

START_TEST(test_push_back_multiple)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 2, 0, 2000, VACANCY_OFFICE);
    Vacancy c = make_vacancy("C", 3, 0, 3000, VACANCY_OFFICE);
    list_push_back(&list, &a);
    list_push_back(&list, &b);
    list_push_back(&list, &c);
    ck_assert_int_eq(list.size, 3);
    ck_assert_str_eq(list.head->data.company, "A");
    ck_assert_str_eq(list.tail->data.company, "C");
    ck_assert_ptr_null(list.head->prev);
    ck_assert_ptr_null(list.tail->next);
    list_free(&list);
}
END_TEST

/* --- list_insert_after --- */

START_TEST(test_insert_after_middle)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 2, 0, 2000, VACANCY_OFFICE);
    Vacancy x = make_vacancy("X", 1, 0, 500,  VACANCY_OFFICE);
    list_push_back(&list, &a);
    list_push_back(&list, &b);
    ck_assert_int_eq(list_insert_after(&list, 0, &x), 1);
    ck_assert_int_eq(list.size, 3);
    ck_assert_str_eq(list.head->next->data.company, "X");
    list_free(&list);
}
END_TEST

START_TEST(test_insert_after_minus1)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy x = make_vacancy("X", 1, 0, 500,  VACANCY_OFFICE);
    list_push_back(&list, &a);
    ck_assert_int_eq(list_insert_after(&list, -1, &x), 1);
    ck_assert_str_eq(list.head->data.company, "X");
    ck_assert_int_eq(list.size, 2);
    list_free(&list);
}
END_TEST

/* --- list_remove --- */

START_TEST(test_remove_first)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 2, 0, 2000, VACANCY_OFFICE);
    list_push_back(&list, &a);
    list_push_back(&list, &b);
    ck_assert_int_eq(list_remove(&list, 0), 1);
    ck_assert_int_eq(list.size, 1);
    ck_assert_str_eq(list.head->data.company, "B");
    ck_assert_ptr_null(list.head->prev);
    list_free(&list);
}
END_TEST

START_TEST(test_remove_last)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 2, 0, 2000, VACANCY_OFFICE);
    list_push_back(&list, &a);
    list_push_back(&list, &b);
    ck_assert_int_eq(list_remove(&list, 1), 1);
    ck_assert_int_eq(list.size, 1);
    ck_assert_str_eq(list.tail->data.company, "A");
    ck_assert_ptr_null(list.tail->next);
    list_free(&list);
}
END_TEST

START_TEST(test_remove_invalid)
{
    VacancyList list;
    list_init(&list);
    Vacancy v = make_vacancy("X", 1, 0, 1000, VACANCY_OFFICE);
    list_push_back(&list, &v);
    ck_assert_int_eq(list_remove(&list, 5), 0);
    ck_assert_int_eq(list_remove(&list, -1), 0);
    ck_assert_int_eq(list.size, 1);
    list_free(&list);
}
END_TEST

/* --- list_filter_exp_and_vacation --- */

START_TEST(test_filter)
{
    VacancyList list;
    list_init(&list);
    Vacancy v1 = make_vacancy("A", 3, 1, 5000, VACANCY_OFFICE);
    Vacancy v2 = make_vacancy("B", 1, 1, 2000, VACANCY_OFFICE);
    Vacancy v3 = make_vacancy("C", 4, 0, 6000, VACANCY_REMOTE);
    Vacancy v4 = make_vacancy("D", 5, 1, 9000, VACANCY_REMOTE);
    list_push_back(&list, &v1);
    list_push_back(&list, &v2);
    list_push_back(&list, &v3);
    list_push_back(&list, &v4);

    VacancyList result;
    list_init(&result);
    list_filter_exp_and_vacation(&list, &result, 2, 1);
    ck_assert_int_eq(result.size, 2);
    list_free(&result);
    list_free(&list);
}
END_TEST

/* --- list_sort_by_salary --- */

START_TEST(test_sort_by_salary)
{
    VacancyList list;
    list_init(&list);
    Vacancy a = make_vacancy("A", 1, 0, 3000, VACANCY_OFFICE);
    Vacancy b = make_vacancy("B", 1, 0, 9000, VACANCY_REMOTE);
    Vacancy c = make_vacancy("C", 1, 0, 5000, VACANCY_OFFICE);
    list_push_back(&list, &a);
    list_push_back(&list, &b);
    list_push_back(&list, &c);
    list_sort_by_salary(&list);
    ck_assert_int_eq(list.head->data.salary.max, 9000);
    ck_assert_int_eq(list.head->next->data.salary.max, 5000);
    ck_assert_int_eq(list.tail->data.salary.max, 3000);
    list_free(&list);
}
END_TEST

/* --- list_is_cyclic --- */

START_TEST(test_not_cyclic)
{
    VacancyList list;
    list_init(&list);
    Vacancy v = make_vacancy("A", 1, 0, 1000, VACANCY_OFFICE);
    list_push_back(&list, &v);
    ck_assert_int_eq(list_is_cyclic(&list), 0);
    list_free(&list);
}
END_TEST

START_TEST(test_empty_not_cyclic)
{
    VacancyList list;
    list_init(&list);
    ck_assert_int_eq(list_is_cyclic(&list), 0);
    list_free(&list);
}
END_TEST

/* --- vacancy_write_file / vacancy_read_arr --- */

START_TEST(test_write_read_file)
{
    VacancyList list;
    list_init(&list);
    Vacancy v1 = make_vacancy("EPAM", 3, 1, 5000, VACANCY_OFFICE);
    strcpy(v1.position, "Junior Java Developer");
    strcpy(v1.english_level, "intermediate");
    strcpy(v1.extra1, "Kharkiv");
    Vacancy v2 = make_vacancy("Luxoft", 5, 1, 9000, VACANCY_REMOTE);
    strcpy(v2.position, "Senior Android Developer");
    strcpy(v2.english_level, "advanced");
    strcpy(v2.extra1, "GMT+2");
    list_push_back(&list, &v1);
    list_push_back(&list, &v2);

    FILE *f = fopen("/tmp/test_lab16.txt", "w");
    list_write_file(&list, f);
    fclose(f);
    list_free(&list);

    f = fopen("/tmp/test_lab16.txt", "r");
    VacancyList list2;
    list_init(&list2);
    list_read_file(&list2, f);
    fclose(f);

    ck_assert_int_eq(list2.size, 2);
    ck_assert_str_eq(list2.head->data.company, "EPAM");
    ck_assert_str_eq(list2.tail->data.company, "Luxoft");
    list_free(&list2);
}
END_TEST

/* --- Suite --- */

Suite *lab16_suite(void) {
    Suite *s = suite_create("Lab16");

    TCase *tc_list = tcase_create("list");
    tcase_add_test(tc_list, test_list_init);
    tcase_add_test(tc_list, test_push_back_single);
    tcase_add_test(tc_list, test_push_back_multiple);
    tcase_add_test(tc_list, test_insert_after_middle);
    tcase_add_test(tc_list, test_insert_after_minus1);
    tcase_add_test(tc_list, test_remove_first);
    tcase_add_test(tc_list, test_remove_last);
    tcase_add_test(tc_list, test_remove_invalid);
    tcase_add_test(tc_list, test_filter);
    tcase_add_test(tc_list, test_sort_by_salary);
    tcase_add_test(tc_list, test_not_cyclic);
    tcase_add_test(tc_list, test_empty_not_cyclic);
    tcase_add_test(tc_list, test_write_read_file);
    suite_add_tcase(s, tc_list);

    return s;
}

int main(void) {
    Suite *s = lab16_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed == 0 ? 0 : 1;
}
