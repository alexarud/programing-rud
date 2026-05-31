/**
 * @file main.c
 * @brief Лабораторна робота №16 — Динамічні списки. Вакансії.
 */
#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "list.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf(" Автор:  Рудь Олександра\n");
    printf(" Група:  КН-925а\n");
    printf(" НТУ «Харківський політехнічний інститут»\n");
    printf(" Лабораторна робота №16\n");
    printf(" Тема:   Динамічні списки\n");
    printf("===========================================\n\n");

    if (argc != 2) {
        fprintf(stderr, "Використання: %s <input.txt>\n", *argv);
        return 1;
    }

    FILE *fin = fopen(*(argv + 1), "r");
    if (!fin) {
        fprintf(stderr, "Помилка: не вдалося відкрити '%s'\n", *(argv + 1));
        return 1;
    }

    VacancyList list;
    list_init(&list);
    list_read_file(&list, fin);
    fclose(fin);

    printf("Завантажено %d вакансій.\n", list.size);

    menu_run(&list, *(argv + 1));

    list_free(&list);
    return 0;
}
