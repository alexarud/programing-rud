/**
 * @file lib.c
 * @brief Реалізація функцій для роботи з вакансіями
 */
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_cmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

static void str_copy(char *dst, const char *src, int max) {
    int i = 0;
    while (i < max - 1 && *(src + i)) {
        *(dst + i) = *(src + i);
        i++;
    }
    *(dst + i) = '\0';
}

int english_level_rank(const char *level) {
    if (str_cmp(level, "pre-intermediate") == 0)  return 0;
    if (str_cmp(level, "intermediate") == 0)       return 1;
    if (str_cmp(level, "upper-intermediate") == 0) return 2;
    if (str_cmp(level, "advanced") == 0)           return 3;
    return -1;
}

static int parse_yes_no(const char *s) {
    return str_cmp(s, "yes") == 0 ? 1 : 0;
}

static void parse_base(Vacancy *v, const char *paid_vac, const char *pos,
                        const char *company, const char *exp,
                        const char *sal_min, const char *sal_max,
                        const char *eng) {
    v->paid_vacation = parse_yes_no(paid_vac);
    str_copy(v->position, pos, MAX_STR);
    str_copy(v->company, company, MAX_STR);
    v->years_exp = atoi(exp);
    v->salary.min = atoi(sal_min);
    v->salary.max = atoi(sal_max);
    str_copy(v->english_level, eng, MAX_STR);
}

static void trim_newline(char *s) {
    int i = 0;
    while (*(s + i))
        i++;
    while (i > 0 && (*(s + i - 1) == '\n' || *(s + i - 1) == '\r')) {
        *(s + i - 1) = '\0';
        i--;
    }
}

void read_vacancies(FILE *f, VacancyCollection *col) {
    col->office_count = 0;
    col->remote_count = 0;

    char line[512];
    while (fgets(line, (int)sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0')
            continue;

        char type[16], paid[8], pos[MAX_STR], company[MAX_STR];
        char exp[8], sal_min[16], sal_max[16], eng[MAX_STR];
        char extra1[MAX_STR], extra2[8];

        int n = sscanf(line, "%15[^;];%7[^;];%63[^;];%63[^;];%7[^;];%15[^;];%15[^;];%63[^;];%63[^;];%7s",
                       type, paid, pos, company, exp, sal_min, sal_max, eng, extra1, extra2);

        if (n < 8)
            continue;

        if (str_cmp(type, "office") == 0 && col->office_count < MAX_VACANCIES) {
            OfficeVacancy *ov = col->office + col->office_count;
            ov->base.type = VACANCY_OFFICE;
            parse_base(&ov->base, paid, pos, company, exp, sal_min, sal_max, eng);
            str_copy(ov->office_address, n >= 9 ? extra1 : "", MAX_STR);
            ov->has_gym = n >= 10 ? parse_yes_no(extra2) : 0;
            col->office_count++;
        } else if (str_cmp(type, "remote") == 0 && col->remote_count < MAX_VACANCIES) {
            RemoteVacancy *rv = col->remote + col->remote_count;
            rv->base.type = VACANCY_REMOTE;
            parse_base(&rv->base, paid, pos, company, exp, sal_min, sal_max, eng);
            str_copy(rv->timezone, n >= 9 ? extra1 : "", MAX_STR);
            rv->remote_exp = n >= 10 ? parse_yes_no(extra2) : 0;
            col->remote_count++;
        }
    }
}

void print_vacancy(const Vacancy *v) {
    printf("  Позиція:    %s\n", v->position);
    printf("  Компанія:   %s\n", v->company);
    printf("  Досвід:     %d р.\n", v->years_exp);
    printf("  Зарплата:   %d–%d\n", v->salary.min, v->salary.max);
    printf("  Англійська: %s\n", v->english_level);
    printf("  Відпустка:  %s\n", v->paid_vacation ? "так" : "ні");
}

void print_office_vacancy(const OfficeVacancy *v) {
    print_vacancy(&v->base);
    printf("  Адреса:     %s\n", v->office_address);
    printf("  Спортзал:   %s\n", v->has_gym ? "так" : "ні");
}

void print_remote_vacancy(const RemoteVacancy *v) {
    print_vacancy(&v->base);
    printf("  Часовий пояс:    %s\n", v->timezone);
    printf("  Досвід remote:   %s\n", v->remote_exp ? "так" : "ні");
}

void filter_exp_and_vacation(const VacancyCollection *col, Vacancy *result, int *count, int min_exp, int paid) {
    *count = 0;
    for (int i = 0; i < col->office_count; i++) {
        const Vacancy *v = &(col->office + i)->base;
        if (v->years_exp > min_exp && (!paid || v->paid_vacation)) {
            *(result + *count) = *v;
            (*count)++;
        }
    }
    for (int i = 0; i < col->remote_count; i++) {
        const Vacancy *v = &(col->remote + i)->base;
        if (v->years_exp > min_exp && (!paid || v->paid_vacation)) {
            *(result + *count) = *v;
            (*count)++;
        }
    }
}

static void collect_all_base(const VacancyCollection *col, Vacancy *arr, int *total) {
    *total = 0;
    for (int i = 0; i < col->office_count; i++) {
        *(arr + *total) = (col->office + i)->base;
        (*total)++;
    }
    for (int i = 0; i < col->remote_count; i++) {
        *(arr + *total) = (col->remote + i)->base;
        (*total)++;
    }
}

void sort_vacancies_by_company(Vacancy *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (str_cmp((arr + i)->company, (arr + j)->company) > 0) {
                Vacancy tmp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = tmp;
            }
        }
    }
}

int sort_and_find_intermediate(const VacancyCollection *col, Vacancy *result) {
    int total = col->office_count + col->remote_count;
    if (total == 0)
        return 0;

    Vacancy arr[MAX_VACANCIES * 2];
    collect_all_base(col, arr, &total);
    sort_vacancies_by_company(arr, total);

    int found = 0;
    for (int i = 0; i < total; i++) {
        if (english_level_rank((arr + i)->english_level) >= 1) {
            *result = *(arr + i);
            found = 1;
            break;
        }
    }

    return found;
}

int find_remote_max_salary(const VacancyCollection *col, RemoteVacancy *result) {
    if (col->remote_count == 0)
        return 0;

    int best = 0;
    for (int i = 1; i < col->remote_count; i++) {
        if ((col->remote + i)->base.salary.max >
            (col->remote + best)->base.salary.max) {
            best = i;
        }
    }
    *result = *(col->remote + best);
    return 1;
}

void write_vacancies(FILE *f, const VacancyCollection *col) {
    for (int i = 0; i < col->office_count; i++) {
        const OfficeVacancy *ov = col->office + i;
        fprintf(f, "office;%s;%s;%s;%d;%d;%d;%s;%s;%s\n",
                ov->base.paid_vacation ? "yes" : "no",
                ov->base.position,
                ov->base.company,
                ov->base.years_exp,
                ov->base.salary.min,
                ov->base.salary.max,
                ov->base.english_level,
                ov->office_address,
                ov->has_gym ? "yes" : "no");
    }
    for (int i = 0; i < col->remote_count; i++) {
        const RemoteVacancy *rv = col->remote + i;
        fprintf(f, "remote;%s;%s;%s;%d;%d;%d;%s;%s;%s\n",
                rv->base.paid_vacation ? "yes" : "no",
                rv->base.position,
                rv->base.company,
                rv->base.years_exp,
                rv->base.salary.min,
                rv->base.salary.max,
                rv->base.english_level,
                rv->timezone,
                rv->remote_exp ? "yes" : "no");
    }
}
