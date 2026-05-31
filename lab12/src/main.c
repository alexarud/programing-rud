/**
 * @file main.c
 * @brief Лабораторна робота №12 — Частотна таблиця символів тексту
 */
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

#define BUF_SIZE 4096

int main(void) {
    printf("===========================================\n");
    printf(" Автор:  Рудь Олександра\n");
    printf(" Група:  КН-925а\n");
    printf(" НТУ «Харківський політехнічний інститут»\n");
    printf(" Лабораторна робота №12\n");
    printf(" Тема:   Рядки (Null-terminated C Strings)\n");
    printf("===========================================\n\n");

    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        fprintf(stderr, "Помилка виділення пам'яті.\n");
        return 1;
    }

    printf("Введіть текст (або передайте через pipe):\n");
    if (!fgets(buf, BUF_SIZE, stdin)) {
        fprintf(stderr, "Помилка читання вводу.\n");
        free(buf);
        return 1;
    }

    int len = str_len(buf);
    while (len > 0 && (*(buf + len - 1) == '\n' || *(buf + len - 1) == '\r')) {
        *(buf + len - 1) = '\0';
        len--;
    }

    if (len == 0) {
        fprintf(stderr, "Помилка: порожній рядок.\n");
        free(buf);
        return 1;
    }

    FreqEntry *entries = malloc((unsigned)len * sizeof(FreqEntry));
    if (!entries) {
        fprintf(stderr, "Помилка виділення пам'яті.\n");
        free(buf);
        return 1;
    }

    int total = 0;
    int unique = build_freq_table(buf, len, entries, &total);
    sort_freq_entries(entries, unique);

    printf("\nТекст: \"%s\"\n", buf);
    printf("Байтів: %d, Символів: %d, Унікальних: %d\n\n", len, total, unique);
    printf("  %-10s  %6s  %10s  %s\n", "Символ", "К-сть", "Частота", "%");
    printf("  %-10s  %6s  %10s  %s\n", "---------", "-----", "-------", "--");

    for (int i = 0; i < unique; i++) {
        FreqEntry *e = entries + i;
        if (e->byte_len == 1 && *(e->bytes) >= 32 && *(e->bytes) < 127) {
            printf("  '%c'         %6d  %10.6f  %.2f%%\n",
                   (char)*(e->bytes), e->count, e->freq, e->freq * 100.0);
        } else {
            printf("  '%s'        %6d  %10.6f  %.2f%%\n",
                   (char *)e->bytes, e->count, e->freq, e->freq * 100.0);
        }
    }

    free(entries);
    free(buf);
    return 0;
}
