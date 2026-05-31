/**
 * @file main.c
 * @brief Лабораторна робота №15 — Динамічні масиви. Вакансії.
 */
#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "list.h"

#define MAX_INPUT 256

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf(" Автор:  Рудь Олександра\n");
    printf(" Група:  КН-925а\n");
    printf(" НТУ «Харківський політехнічний інститут»\n");
    printf(" Лабораторна робота №15\n");
    printf(" Тема:   Динамічні масиви\n");
    printf("===========================================\n\n");

    if (argc != 2) {
        fprintf(stderr, "Використання: %s <input.txt>\n", *argv);
        return 1;
    }

    FILE *fin = fopen(*(argv + 1), "r");
    if (!fin) {
        fprintf(stderr, "Помилка: не вдалося відкрити '%s'\n", *(argv + 1));
        return 1;
    }

    Vacancy buf[MAX_INPUT];
    int n = vacancy_read_file(fin, buf, MAX_INPUT);
    fclose(fin);

    VacancyList list;
    list_init(&list);

    for (int i = 0; i < n; i++)
        list_push(&list, buf + i);

    printf("=== Усі вакансії (%d) ===\n", list.size);
    list_print(&list);

    /* Метод 1: досвід > 2 р. та оплачувана відпустка */
    printf("=== Метод 1: досвід > 2 р. та оплачувана відпустка ===\n");
    VacancyList filtered;
    list_init(&filtered);
    list_filter_exp_and_vacation(&list, &filtered, 2, 1);
    list_print(&filtered);
    list_free(&filtered);

    /* Додавання нової вакансії */
    printf("=== Додавання нової вакансії ===\n");
    Vacancy new_v = {
        VACANCY_REMOTE, 1,
        "Project Manager", "Intellias", 4,
        {3500, 7000}, "upper-intermediate",
        "GMT+2", 1
    };
    list_push(&list, &new_v);
    printf("Додано: %s @ %s\n\n", new_v.position, new_v.company);

    /* Видалення за індексом */
    printf("=== Видалення вакансії за індексом 0 ===\n");
    list_remove(&list, 0);
    printf("Після видалення (%d вакансій):\n\n", list.size);

    /* Сортування за зарплатою */
    printf("=== Сортування за максимальною зарплатою ===\n");
    list_sort_by_salary(&list);
    list_print(&list);

    list_free(&list);
    return 0;
}
