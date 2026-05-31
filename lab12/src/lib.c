/**
 * @file lib.c
 * @brief Реалізація функцій частотного аналізу тексту
 */
#include "lib.h"

int str_len(const char *s) {
    int len = 0;
    while (*(s + len))
        len++;
    return len;
}

int char_len(unsigned char b) {
    if ((b & 0x80u) == 0)     return 1;
    if ((b & 0xE0u) == 0xC0u) return 2;
    if ((b & 0xF0u) == 0xE0u) return 3;
    if ((b & 0xF8u) == 0xF0u) return 4;
    return 1;
}

int char_count(const char *text, int len) {
    int count = 0, i = 0;
    while (i < len) {
        i += char_len((unsigned char)(*(text + i)));
        count++;
    }
    return count;
}

static int bytes_eq(const unsigned char *a, const unsigned char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (*(a + i) != *(b + i))
            return 0;
    }
    return 1;
}

int build_freq_table(const char *text, int len, FreqEntry *entries, int *total) {
    int unique = 0, n = 0, i = 0;

    while (i < len) {
        unsigned char c = (unsigned char)(*(text + i));
        int clen = char_len(c);

        int j;
        for (j = 0; j < unique; j++) {
            if ((entries + j)->byte_len == clen &&
                bytes_eq((entries + j)->bytes,
                         (const unsigned char *)(text + i), clen)) {
                (entries + j)->count++;
                break;
            }
        }
        if (j == unique) {
            FreqEntry *e = entries + unique;
            e->byte_len = clen;
            for (int k = 0; k < clen; k++)
                *(e->bytes + k) = (unsigned char)(*(text + i + k));
            *(e->bytes + clen) = '\0';
            e->count = 1;
            e->freq = 0.0;
            unique++;
        }

        i += clen;
        n++;
    }

    if (n > 0) {
        for (int j = 0; j < unique; j++)
            (entries + j)->freq = (double)(entries + j)->count / (double)n;
    }

    if (total)
        *total = n;

    return unique;
}

void sort_freq_entries(FreqEntry *entries, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((entries + j)->freq > (entries + i)->freq) {
                FreqEntry tmp = *(entries + i);
                *(entries + i) = *(entries + j);
                *(entries + j) = tmp;
            }
        }
    }
}
