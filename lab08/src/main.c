/**
 * @file main.c
 * @brief Приклад програми до лабораторної роботи з теми «Функції».
 *
 * Програма демонструє використання звичайних та варіативних функцій,
 * роботу з масивами й квадратними матрицями, а також обчислення детермінанта.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

/**
 * @brief Перевіряє, чи є число простим.
 *
 * Функція повертає @c true, якщо вхідне число є простим,
 * та @c false, якщо воно складене або менше/дорівнює 1.
 *
 * @param number Ціле число для перевірки.
 * @return true, якщо число просте; false, якщо число не є простим.
 */
bool isPrime (int number){
   if (number <= 1) {
      return false; // числа <=1 не є простими
   }

   for (int i = 2; i < number; i++) {
      if (number % i == 0) {
         return false; // якщо знайдено дільник - число не просте
      }
   }

   return true; // якщо не знайдено дільників - число просте
}

/**
 * @brief Генерує випадкове ціле число у діапазоні [minimum, maximum].
 *
 * Функція використовує стандартну функцію rand(), тому для отримання
 * різних послідовностей чисел при кожному запуску програми слід
 * попередньо викликати srand().
 *
 * @param minimum Нижня межа діапазону (включно).
 * @param maximum Верхня межа діапазону (включно).
 * @return Випадкове ціле число в межах [minimum, maximum].
 *
 * @warning Якщо minimum > maximum, поведінка є невизначеною.
 */
int random_int(int minimum, int maximum){
   return (rand() % (maximum - minimum + 1)) + minimum;
}

/**
 * @brief Виводить матрицю у вигляді таблиці.
 *
 * Матриця зберігається в одновимірному масиві у форматі row-major
 * (спочатку елементи першого рядка, потім другого тощо).
 *
 * @param matrix_array Одновимірний масив елементів матриці.
 * @param rows Кількість рядків матриці.
 * @param cols Кількість стовпців матриці.
 */
void printMatrix (int matrix_array[], int rows, int cols){
   for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
         printf("%d\t", matrix_array[i * cols + j]);
      }
      printf("\n");
   }
}

/**
 * @brief Обчислює квадрат квадратної матриці (A²).
 *
 * Вхідна й вихідна матриці зберігаються в одновимірних масивах у форматі row-major.
 *
 * @param matrix_array Вхідна матриця A у вигляді одновимірного масиву.
 * @param rows Кількість рядків матриці A.
 * @param cols Кількість стовпців матриці A (для квадратної матриці rows == cols).
 * @param result Одновимірний масив для збереження результату множення A².
 *
 * @note Масив @p result має бути попередньо виділений розміром rows * cols.
 */
void squareMatrix (int matrix_array[], int rows, int cols, int result[]){
   for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
         result[i * cols + j] = 0; // спочатку обнуляємо елемент результату
         for (int k = 0; k < cols; k++) {
            // додаємо добуток елементів рядка і стовпця
            result[i * cols + j] += matrix_array[i * cols + k] * matrix_array[k * cols + j];
         }
      }
   }
}

/**
 * @brief Рахує кількість зростаючих пар (i, k), де array[i] < array[k].
 *
 * Функція приймає змінну кількість цілих аргументів, зберігає їх
 * у локальному масиві та рахує кількість пар елементів з індексами
 * i < k, для яких виконується array[i] < array[k].
 *
 * @param count Кількість переданих цілих аргументів.
 * @param ... Список цілих чисел, що утворюють послідовність.
 * @return Кількість зростаючих пар (i, k).
 *
 * @warning Максимальна кількість елементів обмежена розміром локального масиву (1000).
 */
int countIncreasingPairs(int count, ...){
   va_list args;
   va_start(args, count);

   int pairs_count = 0;
   int array_count[1000];

   // Зчитуємо передані аргументи у масив
   for(int i = 0; i < count; i++){
      array_count[i] = va_arg(args, int);
   }

   // Перебираємо всі можливі пари
   for(int i = 0; i < count; i++){
      for(int k = i + 1; k < count; k++){
         if(array_count[i] < array_count[k]){
            pairs_count++; // збільшуємо лічильник, якщо пара зростаюча
         }
      }
   }

   va_end(args);
   return pairs_count;
}

/**
 * @brief Обчислює детермінант квадратної матриці розміром до 3×3.
 *
 * Матриця задається одновимірним масивом у форматі row-major.
 * Підтримуються розміри 1×1, 2×2 та 3×3.
 *
 * @param matrix Одновимірний масив елементів матриці.
 * @param rows Кількість рядків матриці.
 * @param cols Кількість стовпців матриці.
 * @return Значення детермінанта матриці, або 0, якщо:
 * - матриця не квадратна (rows != cols), або
 * - розмір матриці більший за 3×3.
 */
int get_determinant(int matrix[], int rows, int cols){
   if (rows != cols){
      return 0; // детермінант лише для квадратних матриць
   }

   if (rows == 1) {
      return matrix[0]; // для 1x1 детермінант - сам елемент
   }

   if (rows == 2) {
      // |a b|
      // |c d| = ad - bc
      return matrix[0] * matrix[3] - matrix[1] * matrix[2];
   }

   if (rows == 3) {
      // |a b c|
      // |d e f|
      // |g h i|
      int a = matrix[0];
      int b = matrix[1];
      int c = matrix[2];
      int d = matrix[3];
      int e = matrix[4];
      int f = matrix[5];
      int g = matrix[6];
      int h = matrix[7];
      int i = matrix[8];

      // Формула Саррюса для 3x3
      int det = a * (e * i - f * h)
              - b * (d * i - f * g)
              + c * (d * h - e * g);

      return det;
   }

   return 0; // для більших матриць не реалізовано
}

/**
 * @brief Точка входу в програму.
 *
 * Програма:
 * - зчитує число з аргументів командного рядка (або використовує 6 за замовчуванням);
 * - перевіряє це число на простоту за допомогою isPrime();
 * - демонструє роботу варіативної функції countIncreasingPairs();
 * - генерує випадкову квадратну матрицю 3×3;
 * - обчислює її квадрат A² за допомогою squareMatrix();
 * - виводить вихідну матрицю та матрицю A²;
 * - обчислює детермінант вихідної матриці за допомогою get_determinant().
 *
 * @param argc Кількість аргументів командного рядка.
 * @param argv Масив рядків з аргументами командного рядка.
 * @return Код завершення програми (0 — успішне виконання).
 */
int main(int argc, char* argv[]) {
   #define n 3 // розмір квадратної матриці

   int num;

   // Якщо число не передано у параметрах - беремо 6
   if(argc == 1)
      num = 6;
   else
      num = atoi(argv[1]);

   // Виклик перевірки простоти числа
   bool is_prime = isPrime(num);

   if (is_prime)
      printf("%d is prime\n", num);
   else
      printf("%d is not prime\n", num);

   // Виклик варіативної функції для прикладу
   printf("pairs_count: %d\n", countIncreasingPairs(5, 5, 4, 3, 2, 1));

   int matrix[n][n] = {}; // оголошення квадратної матриці n×n

   int min = 0;
   int max = 100;
   srand((unsigned int) time(NULL)); // ініціалізація генератора випадкових чисел

   // Заповнення матриці випадковими значеннями
   for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
         matrix[i][j] = random_int(min, max);
      }
   }

   int matrix_array[n * n];

   // Копіювання двовимірної матриці у одновимірний масив
   for(int i = 0; i < n; i++){
      for(int k = 0; k < n; k++){
         matrix_array[i * n + k] = matrix[i][k];
      }
   }

   int result[n * n]; // результат множення матриці на саму себе

   // Обчислення A²
   squareMatrix(matrix_array, n, n, result);

   // Виведення вихідної матриці
   printf("source matrix:\n");
   printMatrix(matrix_array, n, n);

   // Виведення результату множення
   printf("squared matrix:\n");
   printMatrix(result, n, n);

   // Обчислення детермінанта вихідної матриці
   int det = get_determinant(matrix_array, n, n);
   printf("determinant of source matrix: %d\n", det);

   return 0;
}
