#include <stdbool.h>

int main() {
    int ticket = 894567;
    int ticket_copy = ticket;
    // 
    int digit6 = ticket_copy % 10; 
    ticket_copy = (ticket_copy - digit6)/10;
    int digit5 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit5)/10 ;
    int digit4 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit4) / 10;
    int digit3 = (ticket_copy) % 10;
    ticket_copy = (ticket_copy - digit3) / 10;
    int digit2 = ticket_copy % 10;
    ticket_copy = (ticket_copy - digit2) / 10;
    int digit1 = ticket_copy % 10;

    int left_part = digit1 + digit2 + digit3;
    int right_part = digit4 + digit5 + digit6; 
    
    bool is_lucky = left_part == right_part;

    return 0;
}
