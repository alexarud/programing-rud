/**
 * @file main.c
 * @brief Лабораторна робота №11 — Трикутник Паскаля
 */
#include <stdlib.h>
#include <unistd.h>

#include "lib.h"

static void ws(const char *s) {
    int len = 0;
    while (*(s + len)) len++;
    (void)write(1, s, (unsigned)len);
}

static void wi(int n, int w) {
    char tmp[16];
    int k = 0;
    if (n == 0) {
        tmp[k++] = '0';
    } else {
        int x = n;
        while (x > 0) { tmp[k++] = (char)('0' + x % 10); x /= 10; }
        for (int a = 0, b = k - 1; a < b; a++, b--) {
            char c = tmp[a]; tmp[a] = tmp[b]; tmp[b] = c;
        }
    }
    for (int p = 0; p < w - k; p++) (void)write(1, " ", 1);
    (void)write(1, tmp, (unsigned)k);
}

static int ri(int *out) {
    char buf[64];
    int n = (int)read(0, buf, 63);
    if (n <= 0) return 0;
    *(buf + n) = '\0';
    int val = 0, i = 0, found = 0;
    while (i < n && (*(buf + i) == ' ' || *(buf + i) == '\n')) i++;
    while (i < n && *(buf + i) >= '0' && *(buf + i) <= '9') {
        val = val * 10 + (*(buf + i) - '0');
        found = 1;
        i++;
    }
    if (!found) return 0;
    *out = val;
    return 1;
}

static void print_pascal(int **triangle, int n) {
    for (int i = 0; i < n; i++) {
        int pad = (n - 1 - i) * 2;
        for (int p = 0; p < pad; p++) (void)write(1, " ", 1);
        for (int j = 0; j <= i; j++)
            wi(*(*(triangle + i) + j), 4);
        (void)write(1, "\n", 1);
    }
}

int main(void) {
    ws("===========================================\n");
    ws(" Автор:  Рудь Олександра\n");
    ws(" Група:  КН-925а\n");
    ws(" НТУ «Харківський політехнічний інститут»\n");
    ws(" Лабораторна робота №11\n");
    ws(" Тема:   Матриці. Трикутник Паскаля\n");
    ws("===========================================\n\n");
    ws("Введіть кількість рядків трикутника (1-20): ");

    int n;
    if (!ri(&n)) {
        ws("Помилка: очікується ціле число.\n");
        return 1;
    }
    if (n < 1 || n > 20) {
        ws("Помилка: число рядків має бути від 1 до 20.\n");
        return 1;
    }

    ws("\n--- Ітеративний метод ---\n");
    int **triangle = pascal_iterative(n);
    if (!triangle) {
        ws("Помилка виділення пам'яті.\n");
        return 1;
    }
    print_pascal(triangle, n);
    free_pascal(triangle, n);

    ws("\n--- Рекурсивний метод ---\n");
    for (int i = 0; i < n; i++) {
        int pad = (n - 1 - i) * 2;
        for (int p = 0; p < pad; p++) (void)write(1, " ", 1);
        for (int j = 0; j <= i; j++)
            wi(pascal_recursive(i, j), 4);
        (void)write(1, "\n", 1);
    }

    return 0;
}
