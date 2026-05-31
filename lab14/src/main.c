/**
 * @file main.c
 * @brief Лабораторна робота №14 — Структуровані типи даних. Вакансії.
 */
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf(" Автор:  Рудь Олександра\n");
    printf(" Група:  КН-925а\n");
    printf(" НТУ «Харківський політехнічний інститут»\n");
    printf(" Лабораторна робота №14\n");
    printf(" Тема:   Структуровані типи даних\n");
    printf("===========================================\n\n");

    if (argc != 2) {
        fprintf(stderr, "Використання: %s <input.txt>\n", *argv);
        return 1;
    }

    FILE *fin = fopen(*(argv + 1), "r");
    if (!fin) {
        fprintf(stderr, "Помилка: не вдалося відкрити файл '%s'\n", *(argv + 1));
        return 1;
    }

    VacancyCollection col;
    read_vacancies(fin, &col);
    fclose(fin);

    printf("Зчитано вакансій: офісних — %d, віддалених — %d\n\n",
           col.office_count, col.remote_count);

    /* Метод 1 */
    printf("=== Метод 1: досвід > 2 р. та оплачувана відпустка ===\n");
    Vacancy filtered[MAX_VACANCIES * 2];
    int fcount = 0;
    filter_exp_and_vacation(&col, filtered, &fcount, 2, 1);
    if (fcount == 0) {
        printf("  (нічого не знайдено)\n");
    } else {
        for (int i = 0; i < fcount; i++) {
            printf("  [%d]\n", i + 1);
            print_vacancy(filtered + i);
        }
    }
    printf("\n");

    /* Метод 2 */
    printf("=== Метод 2: перша вакансія (сортування за компанією) з англ. >= Intermediate ===\n");
    Vacancy best;
    if (sort_and_find_intermediate(&col, &best)) {
        print_vacancy(&best);
    } else {
        printf("  (нічого не знайдено)\n");
    }
    printf("\n");

    /* Метод 3 */
    printf("=== Метод 3: віддалена вакансія з найбільшою зарплатою ===\n");
    RemoteVacancy top_remote;
    if (find_remote_max_salary(&col, &top_remote)) {
        print_remote_vacancy(&top_remote);
    } else {
        printf("  (нічого не знайдено)\n");
    }
    printf("\n");

    /* Запис у файл */
    printf("=== Запис у файл dist/output.txt ===\n");
    FILE *fout = fopen("dist/output.txt", "w");
    if (fout) {
        write_vacancies(fout, &col);
        fclose(fout);
        printf("  Збережено.\n");
    } else {
        printf("  Помилка відкриття файлу.\n");
    }

    return 0;
}
