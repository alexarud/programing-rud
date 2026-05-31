/**
 * @file lib.c
 * @brief Реалізація аналогу утиліти tree
 */
#include "lib.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH 4096
#define MAX_PREFIX 1024

void write_to_file(FILE *f, const char *data) {
    fprintf(f, "%s", data);
}

/* Порівняння рядків для qsort */
static int cmp_str(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

void print_tree(const char *path, const char *prefix, FILE *out,
                int *dirs, int *files) {
    DIR *dp = opendir(path);
    if (!dp)
        return;

    /* Збираємо імена записів (крім . та ..) */
    char **names = NULL;
    int count = 0;
    int capacity = 16;
    names = (char **)malloc((size_t)capacity * sizeof(char *));
    if (!names) {
        closedir(dp);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (*(entry->d_name) == '.')
            continue;
        if (count == capacity) {
            capacity *= 2;
            char **tmp = (char **)realloc(names, (size_t)capacity * sizeof(char *));
            if (!tmp) {
                for (int i = 0; i < count; i++)
                    free(*(names + i));
                free(names);
                closedir(dp);
                return;
            }
            names = tmp;
        }
        *(names + count) = (char *)malloc(strlen(entry->d_name) + 1);
        if (!*(names + count))
            continue;
        strcpy(*(names + count), entry->d_name);
        count++;
    }
    closedir(dp);

    /* Сортуємо алфавітно */
    qsort(names, (size_t)count, sizeof(char *), cmp_str);

    for (int i = 0; i < count; i++) {
        int is_last = (i == count - 1);
        const char *branch = is_last ? "`-- " : "|-- ";

        /* Формуємо повний шлях */
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, *(names + i));

        /* Визначаємо чи це каталог */
        struct stat st;
        int is_dir = 0;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
            is_dir = 1;

        /* Виводимо рядок */
        char line[MAX_PATH + MAX_PREFIX];
        snprintf(line, sizeof(line), "%s%s%s\n", prefix, branch, *(names + i));
        write_to_file(stdout, line);
        write_to_file(out, line);

        if (is_dir) {
            (*dirs)++;
            /* Префікс для наступного рівня */
            char new_prefix[MAX_PREFIX];
            snprintf(new_prefix, MAX_PREFIX, "%s%s",
                     prefix, is_last ? "    " : "|   ");
            print_tree(full_path, new_prefix, out, dirs, files);
        } else {
            (*files)++;
        }
    }

    for (int i = 0; i < count; i++)
        free(*(names + i));
    free(names);
}
