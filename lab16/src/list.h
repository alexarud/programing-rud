/**
 * @file list.h
 * @brief Двоспрямований зв'язний список вакансій
 */
#ifndef LIST_H
#define LIST_H

#include "entity.h"

/** @brief Вузол двоспрямованого списку */
typedef struct Node {
    Vacancy      data; /**< Дані вакансії */
    struct Node *next; /**< Вказівник на наступний вузол */
    struct Node *prev; /**< Вказівник на попередній вузол */
} Node;

/** @brief Двоспрямований зв'язний список вакансій */
typedef struct {
    Node *head; /**< Перший вузол */
    Node *tail; /**< Останній вузол */
    int   size; /**< Кількість елементів */
} VacancyList;

/**
 * @brief Ініціалізує порожній список.
 * @param list Вказівник на список.
 */
void list_init(VacancyList *list);

/**
 * @brief Звільняє всю пам'ять списку.
 * @param list Вказівник на список.
 */
void list_free(VacancyList *list);

/**
 * @brief Додає вакансію у кінець списку.
 * @param list Вказівник на список.
 * @param v    Вказівник на вакансію.
 * @return 1 якщо успішно, 0 якщо помилка пам'яті.
 */
int list_push_back(VacancyList *list, const Vacancy *v);

/**
 * @brief Вставляє вакансію після вузла з вказаним індексом.
 *        Якщо index == -1, вставляє на початок.
 * @param list  Вказівник на список.
 * @param after Індекс вузла, після якого вставляти (-1 для початку).
 * @param v     Вказівник на вакансію.
 * @return 1 якщо успішно, 0 якщо помилка.
 */
int list_insert_after(VacancyList *list, int after, const Vacancy *v);

/**
 * @brief Видаляє вузол за індексом.
 * @param list  Вказівник на список.
 * @param index Індекс вузла для видалення.
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

/**
 * @brief Зчитує список з файлу.
 * @param list Вказівник на список.
 * @param f    Вхідний файл.
 */
void list_read_file(VacancyList *list, FILE *f);

/**
 * @brief Записує список у файл.
 * @param list Вказівник на список.
 * @param f    Вихідний файл.
 */
void list_write_file(const VacancyList *list, FILE *f);

/**
 * @brief Перевіряє, чи є список закільцьованим.
 * @param list Вказівник на список.
 * @return 1 якщо закільцьований, 0 якщо ні.
 */
int list_is_cyclic(const VacancyList *list);

#endif
