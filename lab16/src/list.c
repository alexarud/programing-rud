/**
 * @file list.c
 * @brief Реалізація двоспрямованого зв'язного списку вакансій
 */
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void list_init(VacancyList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_free(VacancyList *list) {
    Node *cur = list->head;
    while (cur) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_push_back(VacancyList *list, const Vacancy *v) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return 0;
    node->data = *v;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    list->size++;
    return 1;
}

int list_insert_after(VacancyList *list, int after, const Vacancy *v) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return 0;
    node->data = *v;

    if (after < 0 || list->size == 0) {
        node->next = list->head;
        node->prev = NULL;
        if (list->head)
            list->head->prev = node;
        else
            list->tail = node;
        list->head = node;
        list->size++;
        return 1;
    }

    if (after >= list->size) {
        free(node);
        return 0;
    }

    Node *cur = list->head;
    for (int i = 0; i < after; i++)
        cur = cur->next;

    node->next = cur->next;
    node->prev = cur;
    if (cur->next)
        cur->next->prev = node;
    else
        list->tail = node;
    cur->next = node;
    list->size++;
    return 1;
}

int list_remove(VacancyList *list, int index) {
    if (index < 0 || index >= list->size)
        return 0;

    Node *cur = list->head;
    for (int i = 0; i < index; i++)
        cur = cur->next;

    if (cur->prev)
        cur->prev->next = cur->next;
    else
        list->head = cur->next;

    if (cur->next)
        cur->next->prev = cur->prev;
    else
        list->tail = cur->prev;

    free(cur);
    list->size--;
    return 1;
}

void list_print(const VacancyList *list) {
    if (list->size == 0) {
        printf("  (список порожній)\n");
        return;
    }
    int i = 0;
    Node *cur = list->head;
    while (cur) {
        printf("[%d]\n", i);
        vacancy_print(&cur->data);
        printf("\n");
        cur = cur->next;
        i++;
    }
}

void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result, int min_exp, int paid) {
    Node *cur = list->head;
    while (cur) {
        if (cur->data.years_exp > min_exp && (!paid || cur->data.paid_vacation))
            list_push_back(result, &cur->data);
        cur = cur->next;
    }
}

void list_sort_by_salary(VacancyList *list) {
    if (list->size < 2)
        return;
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        Node *cur = list->head;
        while (cur && cur->next) {
            if (cur->next->data.salary.max > cur->data.salary.max) {
                Vacancy tmp = cur->data;
                cur->data = cur->next->data;
                cur->next->data = tmp;
                swapped = 1;
            }
            cur = cur->next;
        }
    }
}

void list_read_file(VacancyList *list, FILE *f) {
    Vacancy v;
    while (vacancy_read_one(f, &v))
        list_push_back(list, &v);
}

void list_write_file(const VacancyList *list, FILE *f) {
    Node *cur = list->head;
    while (cur) {
        vacancy_write_file(f, &cur->data);
        cur = cur->next;
    }
}

int list_is_cyclic(const VacancyList *list) {
    if (!list->head)
        return 0;
    Node *slow = list->head;
    Node *fast = list->head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return 1;
    }
    return 0;
}
