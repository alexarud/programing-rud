/**
 * @file lib.h
 * @brief Бібліотека для частотного аналізу тексту
 */
#ifndef LIB_H
#define LIB_H

/**
 * @brief Запис частотної таблиці для одного символу.
 */
typedef struct {
    unsigned char bytes[5]; /**< байти символу + '\0' */
    int byte_len;           /**< кількість байтів (1–4) */
    int count;              /**< кількість входжень */
    double freq;            /**< частота [0.0, 1.0] */
} FreqEntry;

/** @brief Повертає довжину рядка у байтах. */
int str_len(const char *s);

/** @brief Повертає кількість байтів символу за першим байтом. */
int char_len(unsigned char first_byte);

/** @brief Повертає кількість символів у рядку. */
int char_count(const char *text, int len);

/**
 * @brief Будує частотну таблицю символів.
 * @param text    Вхідний рядок.
 * @param len     Довжина рядка у байтах.
 * @param entries Масив для запису результату.
 * @param total   [out] Загальна кількість символів.
 * @return Кількість унікальних символів.
 */
int build_freq_table(const char *text, int len, FreqEntry *entries, int *total);

/** @brief Сортує масив FreqEntry за спаданням частоти. */
void sort_freq_entries(FreqEntry *entries, int n);

#endif
