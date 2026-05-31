/**
 * @file lib.h
 * @brief Структури та функції для роботи з вакансіями
 */
#ifndef LIB_H
#define LIB_H

#include <stdio.h>

#define MAX_STR 64
#define MAX_VACANCIES 128

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
    VacancyType   type;
    int           paid_vacation;
    char          position[MAX_STR];
    char          company[MAX_STR];
    int           years_exp;
    SalaryRange   salary;
    char          english_level[MAX_STR];
} Vacancy;

/** @brief Офісна вакансія */
typedef struct {
    Vacancy base;
    char    office_address[MAX_STR];
    int     has_gym;
} OfficeVacancy;

/** @brief Віддалена вакансія */
typedef struct {
    Vacancy base;
    char    timezone[MAX_STR];
    int     remote_exp;
} RemoteVacancy;

/** @brief Колекція вакансій */
typedef struct {
    OfficeVacancy office[MAX_VACANCIES];
    RemoteVacancy remote[MAX_VACANCIES];
    int           office_count;
    int           remote_count;
} VacancyCollection;

/**
 * @brief Зчитує колекцію вакансій з файлу.
 * @param f    Вхідний файл.
 * @param col  Колекція для заповнення.
 */
void read_vacancies(FILE *f, VacancyCollection *col);

/**
 * @brief Виводить базову вакансію.
 * @param v Вказівник на базову вакансію.
 */
void print_vacancy(const Vacancy *v);

/**
 * @brief Виводить офісну вакансію.
 * @param v Вказівник на офісну вакансію.
 */
void print_office_vacancy(const OfficeVacancy *v);

/**
 * @brief Виводить віддалену вакансію.
 * @param v Вказівник на віддалену вакансію.
 */
void print_remote_vacancy(const RemoteVacancy *v);

/**
 * @brief Метод 1: обирає вакансії з досвідом більше min_exp років та оплачуваною відпусткою.
 * @param col      Вхідна колекція.
 * @param result   Масив базових вакансій для результату.
 * @param count    [out] Кількість знайдених вакансій.
 * @param min_exp  Мінімальна кількість років досвіду.
 * @param paid     1 — шукати тільки з оплачуваною відпусткою, 0 — без фільтра.
 */
void filter_exp_and_vacation(const VacancyCollection *col, Vacancy *result, int *count, int min_exp, int paid);

/**
 * @brief Метод 2: сортує всі вакансії за компанією та повертає першу з рівнем англ. >= Intermediate.
 * @param col     Вхідна колекція.
 * @param result  [out] Знайдена вакансія.
 * @return 1 якщо знайдено, 0 якщо ні.
 */
int sort_and_find_intermediate(const VacancyCollection *col, Vacancy *result);

/**
 * @brief Метод 3: знаходить віддалену вакансію з найбільшою максимальною зарплатою.
 * @param col    Вхідна колекція.
 * @param result [out] Знайдена вакансія.
 * @return 1 якщо знайдено, 0 якщо колекція порожня.
 */
int find_remote_max_salary(const VacancyCollection *col, RemoteVacancy *result);

/**
 * @brief Повертає числовий рівень англійської для порівняння.
 * @param level Рядок рівня.
 * @return 0..3 або -1 якщо невідомий.
 */
int english_level_rank(const char *level);

/**
 * @brief Порівнює два рядки. Повертає від'ємне/0/додатнє.
 * @param a Перший рядок.
 * @param b Другий рядок.
 */
int str_cmp(const char *a, const char *b);

/**
 * @brief Записує колекцію вакансій у файл.
 * @param f    Вихідний файл.
 * @param col  Колекція для запису.
 */
void write_vacancies(FILE *f, const VacancyCollection *col);

/**
 * @brief Сортує масив вакансій за назвою компанії алфавітно.
 * @param arr Масив вакансій.
 * @param n   Кількість елементів.
 */
void sort_vacancies_by_company(Vacancy *arr, int n);

#endif
