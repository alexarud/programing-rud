/**
 * @file list.h
 * @brief Динамічний масив вакансій
 */
#ifndef LIST_H
#define LIST_H

#include "entity.h"

/** @brief Динамічний масив вакансій */
typedef struct {
    Vacancy *data;    /**< Вказівник на масив елементів */
    int      size;    /**< Поточна кількість елементів */
    int      capacity; /**< Виділена ємність */
} VacancyList;

/**
 * @brief Ініціалізує порожній список.
 * @param list Вказівник на список.
 */
void list_init(VacancyList *list);

/**
 * @brief Звільняє пам'ять списку.
 * @param list Вказівник на список.
 */
void list_free(VacancyList *list);

/**
 * @brief Додає вакансію у кінець списку.
 * @param list Вказівник на список.
 * @param v    Вказівник на вакансію для додавання.
 * @return 1 якщо успішно, 0 якщо помилка виділення пам'яті.
 */
int list_push(VacancyList *list, const Vacancy *v);

/**
 * @brief Видаляє вакансію зі списку за індексом.
 * @param list  Вказівник на список.
 * @param index Індекс елемента для видалення.
 * @return 1 якщо успішно, 0 якщо індекс виходить за межі.
 */
int list_remove(VacancyList *list, int index);

/**
 * @brief Виводить вміст списку на екран.
 * @param list Вказівник на список.
 */
void list_print(const VacancyList *list);

/**
 * @brief Метод 1: обирає вакансії з досвідом більше min_exp років та оплачуваною відпусткою.
 * @param list     Вхідний список.
 * @param result   Вихідний список (вже ініціалізований).
 * @param min_exp  Мінімальна кількість років досвіду.
 * @param paid     1 — шукати тільки з оплачуваною відпусткою, 0 — без фільтра.
 */
void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result, int min_exp, int paid);

/**
 * @brief Сортує список за максимальною зарплатою (за спаданням).
 * @param list Вказівник на список.
 */
void list_sort_by_salary(VacancyList *list);

#endif
