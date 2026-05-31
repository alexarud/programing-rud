/**
 * @file entity.h
 * @brief Структури для представлення вакансій
 */
#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>

#define MAX_STR 64

/** @brief Діапазон заробітної плати */
typedef struct {
    int min; /**< Нижня межа */
    int max; /**< Верхня межа */
} SalaryRange;

/** @brief Тип вакансії */
typedef enum {
    VACANCY_OFFICE = 0,
    VACANCY_REMOTE = 1
} VacancyType;

/** @brief Базова структура вакансії */
typedef struct {
    VacancyType type;
    int         paid_vacation;
    char        position[MAX_STR];
    char        company[MAX_STR];
    int         years_exp;
    SalaryRange salary;
    char        english_level[MAX_STR];
    char        extra1[MAX_STR]; /**< Адреса офісу або часовий пояс */
    int         extra2;          /**< Наявність спортзалу або досвід remote */
} Vacancy;

/**
 * @brief Виводить вакансію на екран.
 * @param v Вказівник на вакансію.
 */
void vacancy_print(const Vacancy *v);

/**
 * @brief Повертає числовий рівень англійської для порівняння.
 * @param level Рядок рівня.
 * @return 0..3 або -1 якщо невідомий.
 */
int vacancy_english_rank(const char *level);

/**
 * @brief Порівнює два рядки.
 * @param a Перший рядок.
 * @param b Другий рядок.
 * @return Від'ємне/0/додатнє.
 */
int vacancy_str_cmp(const char *a, const char *b);

/**
 * @brief Зчитує вакансії з файлу у масив.
 * @param f     Вхідний файл.
 * @param arr   Масив для заповнення.
 * @param max   Максимальна кількість елементів.
 * @return Кількість зчитаних вакансій.
 */
int vacancy_read_file(FILE *f, Vacancy *arr, int max);

#endif
