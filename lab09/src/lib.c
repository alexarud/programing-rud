#include "lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

// Функція перевіряє, чи є число простим
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

// Генерація випадкового числа у діапазоні [minimum, maximum]
int random_int(int minimum, int maximum){
   return (rand() % (maximum - minimum + 1)) + minimum;
}

// Виведення матриці у вигляді таблиці
void printMatrix (int matrix_array[], int rows, int cols){
   for (int i = 0; i < rows; i++) {        
      for (int j = 0; j < cols; j++) {    
         printf("%d\t", matrix_array[i * cols + j]); 
      }
      printf("\n");  
   } 
} 

// Множення квадратної матриці самої на себе (A²)
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

// Варіативна функція - рахує кількість пар (i, k), де array[i] < array[k]
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

// Обчислення детермінанта матриці до 3x3 включно
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

