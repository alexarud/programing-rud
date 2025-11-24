#ifndef LIB_H
#define LIB_H

#include <stdbool.h>

/**
 * @brief Перевіряє, чи є число простим.
 */
bool isPrime(int number);

/**
 * @brief Генерує випадкове число в діапазоні [minimum, maximum].
 */
int random_int(int minimum, int maximum);

/**
 * @brief Виводить матрицю у вигляді таблиці.
 */
void printMatrix(int matrix_array[], int rows, int cols);

/**
 * @brief Обчислює A² для квадратної матриці.
 */
void squareMatrix(int matrix_array[], int rows, int cols, int result[]);

/**
 * @brief Рахує кількість пар (i, k), де array[i] < array[k].
 */
int countIncreasingPairs(int count, ...);

/**
 * @brief Обчислює детермінант квадратної матриці (до 3x3).
 */
int get_determinant(int matrix[], int rows, int cols);

#endif // LIB_H
