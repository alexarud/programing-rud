/**
 * @file main.c
 * @brief Лабораторна робота №13 — Аналог утиліти tree
 */
#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf(" Автор:  Рудь Олександра\n");
    printf(" Група:  КН-925а\n");
    printf(" НТУ «Харківський політехнічний інститут»\n");
    printf(" Лабораторна робота №13\n");
    printf(" Тема:   Взаємодія з файлами\n");
    printf("===========================================\n\n");

    if (argc != 3) {
        fprintf(stderr, "Використання: %s <шлях_до_каталогу> <output.txt>\n", *argv);
        return 1;
    }

    const char *root = *(argv + 1);
    const char *outpath = *(argv + 2);

    FILE *fout = fopen(outpath, "w");
    if (!fout) {
        fprintf(stderr, "Помилка: не вдалося відкрити файл '%s'\n", outpath);
        return 1;
    }

    /* Виводимо кореневий каталог */
    char root_line[4096];
    snprintf(root_line, sizeof(root_line), "%s\n", root);
    write_to_file(stdout, root_line);
    write_to_file(fout, root_line);

    int dirs = 0, files = 0;
    print_tree(root, "", fout, &dirs, &files);

    /* Підсумок */
    char summary[256];
    snprintf(summary, sizeof(summary),
             "\n%d director%s, %d file%s\n",
             dirs,  dirs  == 1 ? "y" : "ies",
             files, files == 1 ? "" : "s");
    write_to_file(stdout, summary);
    write_to_file(fout, summary);

    fclose(fout);
    return 0;
}
