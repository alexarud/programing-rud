#include <stdbool.h>

int main() {
   int num = 6;              // Число, яке перевіряємо на простоту
   bool is_prime = true;     // Припускаємо, що число просте

   // Перевіряємо дільники від 2 до num - 1
   
   for (int i = 2; i < num; i++) {
      int remainder = num % i;   // Знаходимо остачу від ділення
      if (remainder == 0) {      // Якщо ділиться без остачі — не просте
         is_prime = false;
         break;                  // Далі перевіряти немає сенсу
      }
   }

   // Альтернатива: перевірка через while
   
   // int i = 2;
   // while (i < num) {
   //    if (num % i == 0) {
   //       is_prime = false;
   //       break;
   //    }
   //    i++;
   // }
   

   // Альтернатива: перевірка через do...while

   // int i = 2;
   // do {
   //    if (num % i == 0) {
   //       is_prime = false;
   //       break;
   //    }
   //    i++;
   // } while (i < num);

   return is_prime;          
}
