/**
 * @file lib.h
 * @brief Бібліотека для виведення структури файлів та каталогів (аналог tree)
 */
#ifndef LIB_H
#define LIB_H

#include <stdio.h>

/**
 * @brief Рекурсивно виводить структуру каталогу у вигляді дерева.
 * @param path    Шлях до поточного каталогу.
 * @param prefix  Рядок-префікс для поточного рівня вкладеності.
 * @param out     Вихідний файл (або stdout).
 * @param dirs    [out] Лічильник каталогів.
 * @param files   [out] Лічильник файлів.
 */
void print_tree(const char *path, const char *prefix, FILE *out,
                int *dirs, int *files);

/**
 * @brief Записує рядок у файл через fprintf.
 * @param f    Вихідний файл.
 * @param data Рядок для запису.
 */
void write_to_file(FILE *f, const char *data);

#endif
