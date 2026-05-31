<p align="center">
  <strong>Лабораторна робота №16</strong><br>
  <strong>Тема: Динамічні списки</strong>
</p>

---

## 1. ВИМОГИ

### 1.1. Розробник

- **Студентка:** Рудь Олександра
- **Група:** КН-925а
- **Статус:** студентка НТУ «Харківський політехнічний інститут»
- **Дата розробки:** 25.05.2026

### 1.2. Загальне завдання

Розробити повноцінні звіти для лабораторної роботи **«Динамічні списки»**, що присвячена роботі з динамічними зв'язними списками структур у мові програмування C, у двох форматах:

- **Markdown** (поточний файл `lab16.md`);
- **DOC**-формат, згідно з вимогами **ДСТУ**, з подальшим збереженням у PDF.

При цьому необхідно:

- на базі попередньо розробленого функціоналу (lab15) сформувати односпрямований список елементів структури;
- реалізувати функції: читання/запис файлу, вивід списку, метод №1, додавання, видалення за індексом, сортування;
- реалізувати діалоговий режим спілкування з користувачем за допомогою меню;
- створити коментарі до коду в стилі **Doxygen**;
- продемонструвати відсутність витоків пам'яті за допомогою утиліти **valgrind**;
- забезпечити покриття коду модульними тестами (мінімум 50%);
- доступ до елементів здійснювати через розіменування покажчиків.

### 1.3. Індивідуальне завдання

**Прикладна галузь: Вакансія** (на базі lab15)

Реалізувати двоспрямований зв'язний список вакансій з такими операціями:
- виведення вмісту списку;
- метод №1 — обрати всі вакансії з досвідом > 2 років та оплачуваною відпусткою;
- додавання вакансії у кінець списку;
- вставка вакансії після будь-якого елемента (додаткове завдання);
- видалення вакансії зі списку за індексом;
- сортування списку за максимальною зарплатою (за спаданням);
- читання з файлу та запис у файл;
- перевірка чи є список закільцьованим (додаткове завдання).

---

## 2. ОПИС ПРОГРАМИ

### 2.1. Функціональне призначення

Програма призначена для роботи з двоспрямованим зв'язним списком вакансій у діалоговому режимі.

Основні можливості:
1. Зчитати вакансії з вхідного файлу у зв'язний список.
2. Відобразити інтерактивне меню.
3. Виконати будь-яку з 8 операцій зі списком.
4. Зберегти результат у файл.
5. Коректно звільнити всю динамічно виділену пам'ять.

**Обмеження на застосування:**
- програма не очікує вводу без меню;
- вихідний файл перезаписує вхідний.

---

### 2.2. Опис логічної структури

#### 2.2.1. Структури даних

```c
typedef struct Node {
    Vacancy      data; /* дані вакансії */
    struct Node *next; /* вказівник на наступний вузол */
    struct Node *prev; /* вказівник на попередній вузол */
} Node;

typedef struct {
    Node *head; /* перший вузол */
    Node *tail; /* останній вузол */
    int   size; /* кількість елементів */
} VacancyList;
```

#### 2.2.2. Структура функцій

**Модуль entity (`entity.h` / `entity.c`):**

- `int vacancy_str_cmp(const char *a, const char *b)`
  Порівнює два рядки побайтово через розіменування покажчиків.

- `int vacancy_english_rank(const char *level)`
  Повертає числовий рівень англійської: 0..3 або -1.

- `void vacancy_print(const Vacancy *v)`
  Виводить усі поля вакансії на екран.

- `int vacancy_read_arr(FILE *f, Vacancy *arr, int max)`
  Зчитує вакансії з файлу у масив за допомогою `fgets` + `sscanf`.

- `void vacancy_write_file(FILE *f, const Vacancy *v)`
  Записує вакансію у файл за допомогою `fprintf`.

**Модуль list (`list.h` / `list.c`):**

- `void list_init(VacancyList *list)`
  Ініціалізує порожній список.

- `void list_free(VacancyList *list)`
  Звільняє всі вузли списку.

- `int list_push_back(VacancyList *list, const Vacancy *v)`
  Додає вакансію у кінець. Виділяє новий вузол через `malloc`.

- `int list_insert_after(VacancyList *list, int after, const Vacancy *v)`
  Вставляє вакансію після вузла з індексом `after`. При `after == -1` вставляє на початок.

- `int list_remove(VacancyList *list, int index)`
  Видаляє вузол за індексом, оновлює вказівники сусідніх вузлів.

- `void list_print(const VacancyList *list)`
  Виводить усі вакансії з їхніми індексами.

- `void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result)`
  Метод №1: копіює до `result` вакансії з досвідом > 2 р. та оплачуваною відпусткою.

- `void list_sort_by_salary(VacancyList *list)`
  Сортує список за спаданням максимальної зарплати методом бульбашки (swap даних).

- `void list_read_file(VacancyList *list, FILE *f)`
  Зчитує вакансії з файлу у список.

- `void list_write_file(const VacancyList *list, FILE *f)`
  Записує список у файл.

- `int list_is_cyclic(const VacancyList *list)`
  Перевіряє закільцьованість алгоритмом Флойда (два вказівники: повільний і швидкий).

**Модуль menu (`menu.h` / `menu.c`):**

- `void menu_print(void)`
  Виводить меню на екран.

- `void menu_run(VacancyList *list, const char *filename)`
  Запускає діалоговий цикл. Зчитує вибір користувача та викликає відповідну функцію.

#### 2.2.3. Структура програми

```text
lab16/
├── Doxyfile
├── Makefile
├── assets/
│   └── input.txt
├── doc/
│   └── lab16.md
├── src/
│   ├── entity.c
│   ├── entity.h
│   ├── list.c
│   ├── list.h
│   ├── menu.c
│   ├── menu.h
│   └── main.c
└── test/
    └── test.c
```

### 2.3. Опис та реалізація функцій

#### 2.3.1. Функція list_push_back

**Призначення:** Додає нову вакансію у кінець двоспрямованого списку. Виділяє пам'ять під новий вузол, встановлює вказівники `prev` і `next`.

**Код:**
```c
int list_push_back(VacancyList *list, const Vacancy *v) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return 0;
    node->data = *v;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    list->size++;
    return 1;
}
```

#### 2.3.2. Функція list_insert_after

**Призначення:** Вставляє вакансію після вузла з вказаним індексом. При `after == -1` вставляє на початок списку. Оновлює вказівники `prev`/`next` сусідніх вузлів.

**Код:**
```c
int list_insert_after(VacancyList *list, int after, const Vacancy *v) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return 0;
    node->data = *v;

    if (after < 0 || list->size == 0) {
        node->next = list->head;
        node->prev = NULL;
        if (list->head)
            list->head->prev = node;
        else
            list->tail = node;
        list->head = node;
        list->size++;
        return 1;
    }

    if (after >= list->size) {
        free(node);
        return 0;
    }

    Node *cur = list->head;
    for (int i = 0; i < after; i++)
        cur = cur->next;

    node->next = cur->next;
    node->prev = cur;
    if (cur->next)
        cur->next->prev = node;
    else
        list->tail = node;
    cur->next = node;
    list->size++;
    return 1;
}
```

#### 2.3.3. Функція list_remove

**Призначення:** Видаляє вузол за індексом. Оновлює вказівники `prev`/`next` сусідніх вузлів та `head`/`tail` списку. Звільняє пам'ять вузла.

**Код:**
```c
int list_remove(VacancyList *list, int index) {
    if (index < 0 || index >= list->size)
        return 0;

    Node *cur = list->head;
    for (int i = 0; i < index; i++)
        cur = cur->next;

    if (cur->prev)
        cur->prev->next = cur->next;
    else
        list->head = cur->next;

    if (cur->next)
        cur->next->prev = cur->prev;
    else
        list->tail = cur->prev;

    free(cur);
    list->size--;
    return 1;
}
```

#### 2.3.4. Функція list_filter_exp_and_vacation

**Призначення:** Метод №1 — обирає вакансії з досвідом більше 2 років та оплачуваною відпусткою.

**Код:**
```c
void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result) {
    Node *cur = list->head;
    while (cur) {
        if (cur->data.years_exp > 2 && cur->data.paid_vacation)
            list_push_back(result, &cur->data);
        cur = cur->next;
    }
}
```

#### 2.3.5. Функція list_sort_by_salary

**Призначення:** Сортує список за спаданням максимальної зарплати методом бульбашки. Замість перелінкування вузлів — swap даних між сусідніми вузлами.

**Код:**
```c
void list_sort_by_salary(VacancyList *list) {
    if (list->size < 2)
        return;
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        Node *cur = list->head;
        while (cur && cur->next) {
            if (cur->next->data.salary.max > cur->data.salary.max) {
                Vacancy tmp = cur->data;
                cur->data = cur->next->data;
                cur->next->data = tmp;
                swapped = 1;
            }
            cur = cur->next;
        }
    }
}
```

#### 2.3.6. Функція list_is_cyclic

**Призначення:** Перевіряє, чи є список закільцьованим, за допомогою алгоритму Флойда. Два вказівники рухаються з різними швидкостями: якщо зустрілись — список закільцьований.

**Код:**
```c
int list_is_cyclic(const VacancyList *list) {
    if (!list->head)
        return 0;
    Node *slow = list->head;
    Node *fast = list->head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return 1;
    }
    return 0;
}
```

---

## 3. ВАРІАНТИ ВИКОРИСТАННЯ

**Компіляція та запуск:**
```bash
make run
```

**Або вручну:**
```bash
./dist/main.bin "./assets/input.txt"
```

**Приклад роботи програми:**

```
===========================================
 Автор:  Рудь Олександра
 Група:  КН-925а
 НТУ «Харківський політехнічний інститут»
 Лабораторна робота №16
 Тема:   Динамічні списки
===========================================

Завантажено 8 вакансій.

=== МЕНЮ ===
1. Вивести список
2. Додати вакансію у кінець
3. Вставити вакансію після елемента
4. Видалити вакансію за індексом
5. Метод 1: досвід > 2 р. та оплачувана відпустка
6. Сортувати за зарплатою
7. Зберегти у файл
8. Перевірити чи список закільцьований
0. Вийти
Ваш вибір:
```

**Перевірка на витоки пам'яті:**
```bash
make leak-check
```

**Запуск тестів:**
```bash
make test
```

**Покриття коду тестами:**
```bash
make coverage
```

---

## ВИСНОВКИ

У ході виконання лабораторної роботи було:
- Реалізовано двоспрямований зв'язний список вакансій `VacancyList` на основі структури `Node` з вказівниками `next` та `prev`.
- Реалізовано операції: додавання у кінець, вставка після довільного елемента, видалення за індексом, фільтрація за критерієм, сортування за зарплатою.
- Реалізовано читання списку з файлу (`fscanf`-сумісний формат) та запис у файл через `fprintf`.
- Реалізовано діалогове меню для демонстрації всіх операцій.
- Реалізовано перевірку закільцьованості списку за алгоритмом Флойда.
- Доступ до елементів здійснюється виключно через розіменування покажчиків.
- Проведено перевірку на витоки пам'яті за допомогою `valgrind` (витоків не виявлено).
- Написано 13 модульних тестів з покриттям коду понад 50%.
- Оформлено документацію Doxygen.

Мета роботи досягнута, програма працює коректно та відповідає поставленому завданню.
