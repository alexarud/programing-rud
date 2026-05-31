/**
 * @file menu.c
 * @brief Реалізація діалогового меню
 */
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu_print(void) {
    printf("\n=== МЕНЮ ===\n");
    printf("1. Вивести список\n");
    printf("2. Додати вакансію у кінець\n");
    printf("3. Вставити вакансію після елемента\n");
    printf("4. Видалити вакансію за індексом\n");
    printf("5. Метод 1: досвід > 2 р. та оплачувана відпустка\n");
    printf("6. Сортувати за зарплатою\n");
    printf("7. Зберегти у файл\n");
    printf("8. Перевірити чи список закільцьований\n");
    printf("0. Вийти\n");
    printf("Ваш вибір: ");
}

static void read_vacancy_from_input(Vacancy *v) {
    char type[16], paid[8], extra2[8];
    printf("Тип (office/remote): ");
    if (scanf("%15s", type) != 1) return;
    v->type = (strcmp(type, "office") == 0) ? VACANCY_OFFICE : VACANCY_REMOTE;

    printf("Оплачувана відпустка (yes/no): ");
    if (scanf("%7s", paid) != 1) return;
    v->paid_vacation = (strcmp(paid, "yes") == 0) ? 1 : 0;

    printf("Позиція: ");
    if (scanf(" %63[^\n]", v->position) != 1) return;

    printf("Компанія: ");
    if (scanf(" %63[^\n]", v->company) != 1) return;

    printf("Досвід (років): ");
    if (scanf("%d", &v->years_exp) != 1) return;

    printf("Зарплата мін: ");
    if (scanf("%d", &v->salary.min) != 1) return;

    printf("Зарплата макс: ");
    if (scanf("%d", &v->salary.max) != 1) return;

    printf("Рівень англійської (pre-intermediate/intermediate/upper-intermediate/advanced): ");
    if (scanf(" %63[^\n]", v->english_level) != 1) return;

    if (v->type == VACANCY_OFFICE) {
        printf("Адреса офісу: ");
        if (scanf(" %63[^\n]", v->extra1) != 1) return;
        printf("Спортзал (yes/no): ");
    } else {
        printf("Часовий пояс: ");
        if (scanf(" %63[^\n]", v->extra1) != 1) return;
        printf("Досвід remote (yes/no): ");
    }
    if (scanf("%7s", extra2) != 1) return;
    v->extra2 = (strcmp(extra2, "yes") == 0) ? 1 : 0;
}

void menu_run(VacancyList *list, const char *filename) {
    int choice;
    while (1) {
        menu_print();
        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
        case 0:
            printf("До побачення!\n");
            return;
        case 1:
            list_print(list);
            break;
        case 2: {
            Vacancy v;
            read_vacancy_from_input(&v);
            if (list_push_back(list, &v))
                printf("Додано.\n");
            else
                printf("Помилка пам'яті.\n");
            break;
        }
        case 3: {
            int after;
            printf("Після якого індексу (-1 для початку): ");
            if (scanf("%d", &after) != 1) continue;
            Vacancy v;
            read_vacancy_from_input(&v);
            if (list_insert_after(list, after, &v))
                printf("Вставлено.\n");
            else
                printf("Невірний індекс.\n");
            break;
        }
        case 4: {
            int idx;
            printf("Індекс для видалення: ");
            if (scanf("%d", &idx) != 1) continue;
            if (list_remove(list, idx))
                printf("Видалено.\n");
            else
                printf("Невірний індекс.\n");
            break;
        }
        case 5: {
            VacancyList result;
            list_init(&result);
            list_filter_exp_and_vacation(list, &result, 2, 1);
            printf("=== Результат фільтрації (%d) ===\n", result.size);
            list_print(&result);
            list_free(&result);
            break;
        }
        case 6:
            list_sort_by_salary(list);
            printf("Відсортовано.\n");
            list_print(list);
            break;
        case 7: {
            FILE *f = fopen(filename, "w");
            if (!f) {
                printf("Помилка відкриття файлу.\n");
                continue;
            }
            list_write_file(list, f);
            fclose(f);
            printf("Збережено у %s\n", filename);
            break;
        }
        case 8:
            printf("Список %s\n", list_is_cyclic(list) ? "закільцьований" : "не закільцьований");
            break;
        default:
            printf("Невідома команда.\n");
            break;
        }
    }
}
