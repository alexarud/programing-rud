#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "lib.h"


// --- Основна функція програми ---
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
   printf("pairs_count: %d\n", countIncreasingPairs(5, 1, 2, 3, 2, 1));

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
