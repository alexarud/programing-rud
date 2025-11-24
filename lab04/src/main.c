#include <stdbool.h>

int main() {
    int ticket = 1231151101;        // Номер квитка (6 цифр)
    int ticket_copy = ticket;   // Копія, щоб не втратити початкове значення під час ділення
    
    // Виділяємо цифри квитка, починаючи з кінця

    int digit10 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit10) / 10;
    int digit9 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit9) / 10;

    int digit8 = ticket_copy % 10; 
    ticket_copy = (ticket_copy - digit8) / 10; 
    int digit7 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit7) / 10;

    int digit6 = ticket_copy % 10; 
    ticket_copy = (ticket_copy - digit6) / 10;
    int digit5 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit5) / 10;
    int digit4 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit4) / 10;
    int digit3 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit3) / 10;
    int digit2 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit2) / 10;
    int digit1 = ticket_copy % 10;

    // Обчислюємо суми першої та другої половини квитка
    int left_part = digit1 + digit2 + digit3 + digit4 + digit5;
    int right_part = digit6 + digit7 + digit8 + digit9 + digit10; 
    
    // Порівнюємо суми та повертаємо результат:
    // 1 — щасливий квиток, 0 — ні

    // if (left_part == right_part)
    //     return 1;
    // else
    //     return 0;

    bool is_lucky = left_part == right_part;
        return 0;
}
