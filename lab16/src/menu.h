/**
 * @file menu.h
 * @brief Діалогове меню для роботи зі списком вакансій
 */
#ifndef MENU_H
#define MENU_H

#include "list.h"

/**
 * @brief Виводить головне меню на екран.
 */
void menu_print(void);

/**
 * @brief Запускає діалоговий цикл меню.
 * @param list Вказівник на список вакансій.
 * @param filename Шлях до файлу для збереження.
 */
void menu_run(VacancyList *list, const char *filename);

#endif
