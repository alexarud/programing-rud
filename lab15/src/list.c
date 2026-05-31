/**
 * @file list.c
 * @brief Реалізація динамічного масиву вакансій
 */
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 4

void list_init(VacancyList *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void list_free(VacancyList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int list_push(VacancyList *list, const Vacancy *v) {
    if (list->size == list->capacity) {
        int new_cap = list->capacity == 0 ? INIT_CAPACITY : list->capacity * 2;
        Vacancy *new_data = (Vacancy *)realloc(list->data,
                                               (size_t)new_cap * sizeof(Vacancy));
        if (!new_data)
            return 0;
        list->data = new_data;
        list->capacity = new_cap;
    }
    *(list->data + list->size) = *v;
    list->size++;
    return 1;
}

int list_remove(VacancyList *list, int index) {
    if (index < 0 || index >= list->size)
        return 0;
    for (int i = index; i < list->size - 1; i++)
        *(list->data + i) = *(list->data + i + 1);
    list->size--;
    return 1;
}

void list_print(const VacancyList *list) {
    if (list->size == 0) {
        printf("  (список порожній)\n");
        return;
    }
    for (int i = 0; i < list->size; i++) {
        printf("[%d]\n", i);
        vacancy_print(list->data + i);
        printf("\n");
    }
}

void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result, int min_exp, int paid) {
    for (int i = 0; i < list->size; i++) {
        const Vacancy *v = list->data + i;
        if (v->years_exp > min_exp && (!paid || v->paid_vacation))
            list_push(result, v);
    }
}

void list_sort_by_salary(VacancyList *list) {
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if ((list->data + j)->salary.max > (list->data + i)->salary.max) {
                Vacancy tmp = *(list->data + i);
                *(list->data + i) = *(list->data + j);
                *(list->data + j) = tmp;
            }
        }
    }
}
