#include "lib.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* --- write_to_file --- */

START_TEST(test_write_to_file_basic)
{
    FILE *f = fopen("/tmp/lab13_test_write.txt", "w");
    write_to_file(f, "hello\n");
    fclose(f);

    f = fopen("/tmp/lab13_test_write.txt", "r");
    char buf[64];
    fgets(buf, 64, f);
    fclose(f);

    ck_assert_str_eq(buf, "hello\n");
}
END_TEST

START_TEST(test_write_to_file_empty)
{
    FILE *f = fopen("/tmp/lab13_test_empty.txt", "w");
    write_to_file(f, "");
    fclose(f);

    f = fopen("/tmp/lab13_test_empty.txt", "r");
    char buf[64];
    buf[0] = 'x';
    int r = (int)(fread(buf, 1, 63, f) == 0);
    fclose(f);

    ck_assert_int_eq(r, 1);
}
END_TEST

/* --- print_tree --- */

/* Створює тимчасову структуру каталогів для тестів */
static void create_test_tree(void) {
    mkdir("/tmp/lab13_tree", 0755);
    mkdir("/tmp/lab13_tree/subdir", 0755);

    FILE *f = fopen("/tmp/lab13_tree/file1.txt", "w");
    if (f) { fprintf(f, "a"); fclose(f); }

    f = fopen("/tmp/lab13_tree/file2.txt", "w");
    if (f) { fprintf(f, "b"); fclose(f); }

    f = fopen("/tmp/lab13_tree/subdir/nested.txt", "w");
    if (f) { fprintf(f, "c"); fclose(f); }
}

START_TEST(test_print_tree_counts)
{
    create_test_tree();

    FILE *f = fopen("/tmp/lab13_tree_out.txt", "w");
    int dirs = 0, files = 0;
    print_tree("/tmp/lab13_tree", "", f, &dirs, &files);
    fclose(f);

    /* 1 підкаталог subdir, 2 файли у корені + 1 у subdir = 3 файли */
    ck_assert_int_eq(dirs, 1);
    ck_assert_int_eq(files, 3);
}
END_TEST

START_TEST(test_print_tree_output_contains_names)
{
    create_test_tree();

    FILE *f = fopen("/tmp/lab13_tree_out2.txt", "w");
    int dirs = 0, files = 0;
    print_tree("/tmp/lab13_tree", "", f, &dirs, &files);
    fclose(f);

    f = fopen("/tmp/lab13_tree_out2.txt", "r");
    char content[2048];
    content[0] = '\0';
    char buf[256];
    while (fgets(buf, 256, f))
        strncat(content, buf, sizeof(content) - strlen(content) - 1);
    fclose(f);

    ck_assert(strstr(content, "file1.txt") != NULL);
    ck_assert(strstr(content, "file2.txt") != NULL);
    ck_assert(strstr(content, "subdir")    != NULL);
    ck_assert(strstr(content, "nested.txt") != NULL);
}
END_TEST

START_TEST(test_print_tree_empty_dir)
{
    mkdir("/tmp/lab13_empty", 0755);

    FILE *f = fopen("/tmp/lab13_empty_out.txt", "w");
    int dirs = 0, files = 0;
    print_tree("/tmp/lab13_empty", "", f, &dirs, &files);
    fclose(f);

    ck_assert_int_eq(dirs, 0);
    ck_assert_int_eq(files, 0);
}
END_TEST

START_TEST(test_print_tree_invalid_path)
{
    FILE *f = fopen("/tmp/lab13_invalid_out.txt", "w");
    int dirs = 0, files = 0;
    /* Не існуючий шлях — не повинен падати */
    print_tree("/tmp/lab13_nonexistent_xyz", "", f, &dirs, &files);
    fclose(f);

    ck_assert_int_eq(dirs, 0);
    ck_assert_int_eq(files, 0);
}
END_TEST

/* --- Suite --- */

Suite *lab13_suite(void) {
    Suite *s = suite_create("Lab13");

    TCase *tc_io = tcase_create("write_to_file");
    tcase_add_test(tc_io, test_write_to_file_basic);
    tcase_add_test(tc_io, test_write_to_file_empty);
    suite_add_tcase(s, tc_io);

    TCase *tc_tree = tcase_create("print_tree");
    tcase_add_test(tc_tree, test_print_tree_counts);
    tcase_add_test(tc_tree, test_print_tree_output_contains_names);
    tcase_add_test(tc_tree, test_print_tree_empty_dir);
    tcase_add_test(tc_tree, test_print_tree_invalid_path);
    suite_add_tcase(s, tc_tree);

    return s;
}

int main(void) {
    Suite *s = lab13_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed == 0 ? 0 : 1;
}
