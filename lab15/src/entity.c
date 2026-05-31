/**
 * @file entity.c
 * @brief Реалізація функцій для роботи з вакансіями
 */
#include "entity.h"

#include <stdio.h>
#include <stdlib.h>

int vacancy_str_cmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int vacancy_english_rank(const char *level) {
    if (vacancy_str_cmp(level, "pre-intermediate") == 0)  return 0;
    if (vacancy_str_cmp(level, "intermediate") == 0)      return 1;
    if (vacancy_str_cmp(level, "upper-intermediate") == 0) return 2;
    if (vacancy_str_cmp(level, "advanced") == 0)          return 3;
    return -1;
}

void vacancy_print(const Vacancy *v) {
    printf("  Тип:        %s\n", v->type == VACANCY_OFFICE ? "офісна" : "віддалена");
    printf("  Позиція:    %s\n", v->position);
    printf("  Компанія:   %s\n", v->company);
    printf("  Досвід:     %d р.\n", v->years_exp);
    printf("  Зарплата:   %d-%d\n", v->salary.min, v->salary.max);
    printf("  Англійська: %s\n", v->english_level);
    printf("  Відпустка:  %s\n", v->paid_vacation ? "так" : "ні");
    if (v->type == VACANCY_OFFICE) {
        printf("  Адреса:     %s\n", v->extra1);
        printf("  Спортзал:   %s\n", v->extra2 ? "так" : "ні");
    } else {
        printf("  Часовий пояс:  %s\n", v->extra1);
        printf("  Досвід remote: %s\n", v->extra2 ? "так" : "ні");
    }
}

static void str_copy(char *dst, const char *src, int max) {
    int i = 0;
    while (i < max - 1 && *(src + i)) {
        *(dst + i) = *(src + i);
        i++;
    }
    *(dst + i) = '\0';
}

static void trim_newline(char *s) {
    int i = 0;
    while (*(s + i)) i++;
    while (i > 0 && (*(s + i - 1) == '\n' || *(s + i - 1) == '\r')) {
        *(s + i - 1) = '\0';
        i--;
    }
}

int vacancy_read_file(FILE *f, Vacancy *arr, int max) {
    int count = 0;
    char line[512];
    while (count < max && fgets(line, (int)sizeof(line), f)) {
        trim_newline(line);
        if (*line == '\0') continue;

        char type[16], paid[8], pos[MAX_STR], company[MAX_STR];
        char exp[8], sal_min[16], sal_max[16], eng[MAX_STR];
        char extra1[MAX_STR], extra2[8];

        int n = sscanf(line,
            "%15[^;];%7[^;];%63[^;];%63[^;];%7[^;];%15[^;];%15[^;];%63[^;];%63[^;];%7s",
            type, paid, pos, company, exp, sal_min, sal_max, eng, extra1, extra2);

        if (n < 8) continue;

        Vacancy *v = arr + count;
        v->type = (vacancy_str_cmp(type, "office") == 0) ? VACANCY_OFFICE : VACANCY_REMOTE;
        v->paid_vacation = (vacancy_str_cmp(paid, "yes") == 0) ? 1 : 0;
        str_copy(v->position, pos, MAX_STR);
        str_copy(v->company, company, MAX_STR);
        v->years_exp = atoi(exp);
        v->salary.min = atoi(sal_min);
        v->salary.max = atoi(sal_max);
        str_copy(v->english_level, eng, MAX_STR);
        str_copy(v->extra1, n >= 9 ? extra1 : "", MAX_STR);
        v->extra2 = (n >= 10 && vacancy_str_cmp(extra2, "yes") == 0) ? 1 : 0;
        count++;
    }
    return count;
}
