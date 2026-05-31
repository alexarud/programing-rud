<p align="center">
  <strong>Лабораторна робота №15</strong><br>
  <strong>Тема: Динамічні масиви</strong>
</p>

---

## 1. ВИМОГИ

### 1.1. Розробник

- **Студентка:** Рудь Олександра
- **Група:** КН-925а
- **Статус:** студентка НТУ «Харківський політехнічний інститут»
- **Дата розробки:** 18.05.2026

### 1.2. Загальне завдання

Розробити повноцінні звіти для лабораторної роботи **«Динамічні масиви»**, що присвячена роботі з динамічно виділеними масивами структур у мові програмування C, у двох форматах:

- **Markdown** (поточний файл `lab15.md`);
- **DOC**-формат, згідно з вимогами **ДСТУ**, з подальшим збереженням у PDF.

При цьому необхідно:

- на базі попередньо розробленого функціоналу (lab14) сформувати динамічний масив елементів структури;
- реалізувати функції: виведення вмісту списку, метод №1 з колекції, додавання у кінець, видалення за індексом, сортування;
- створити коментарі до коду в стилі **Doxygen**;
- продемонструвати відсутність витоків пам'яті за допомогою утиліти **valgrind**;
- забезпечити покриття коду модульними тестами (мінімум 50%);
- доступ до елементів масиву здійснювати через розіменування покажчиків.

### 1.3. Індивідуальне завдання

**Прикладна галузь: Вакансія** (на базі lab14)

Реалізувати динамічний масив вакансій з такими операціями:
- виведення вмісту списку;
- метод №1 — обрати всі вакансії з досвідом > 2 років та оплачуваною відпусткою;
- додавання вакансії у кінець списку;
- видалення вакансії зі списку за індексом;
- сортування списку за максимальною зарплатою (за спаданням).

---

## 2. ОПИС ПРОГРАМИ

### 2.1. Функціональне призначення

Програма призначена для роботи з динамічним масивом вакансій.

Основні можливості:
1. Зчитати вакансії з вхідного файлу у динамічний масив.
2. Вивести вміст масиву.
3. Відфільтрувати вакансії за досвідом та наявністю відпустки.
4. Додати нову вакансію у кінець масиву.
5. Видалити вакансію за індексом.
6. Відсортувати масив за максимальною зарплатою.
7. Коректно звільнити всю динамічно виділену пам'ять.

**Обмеження на застосування:**
- початкова ємність масиву — 4 елементи, подвоюється при переповненні;
- програма не очікує жодного вводу від користувача.

### 2.2. Опис логічної структури

#### 2.2.1. Структури даних

```c
typedef struct {
    int min;
    int max;
} SalaryRange;

typedef struct {
    VacancyType type;
    int         paid_vacation;
    char        position[64];
    char        company[64];
    int         years_exp;
    SalaryRange salary;
    char        english_level[64];
    char        extra1[64];
    int         extra2;
} Vacancy;

typedef struct {
    Vacancy *data;
    int      size;
    int      capacity;
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

- `int vacancy_read_file(FILE *f, Vacancy *arr, int max)`  
  Зчитує вакансії з файлу, повертає кількість зчитаних.

**Модуль list (`list.h` / `list.c`):**

- `void list_init(VacancyList *list)`  
  Ініціалізує порожній список.

- `void list_free(VacancyList *list)`  
  Звільняє пам'ять масиву.

- `int list_push(VacancyList *list, const Vacancy *v)`  
  Додає вакансію у кінець. При переповненні подвоює ємність через `realloc`.

- `int list_remove(VacancyList *list, int index)`  
  Видаляє елемент за індексом, зсуваючи наступні через розіменування покажчиків.

- `void list_print(const VacancyList *list)`  
  Виводить усі вакансії з їхніми індексами.

- `void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result)`  
  Метод №1: копіює до `result` вакансії з досвідом > 2 р. та оплачуваною відпусткою.

- `void list_sort_by_salary(VacancyList *list)`  
  Сортує список за спаданням максимальної зарплати методом обміну.

- `int main(int argc, char *argv[])`  
  Точка входу. Зчитує файл, демонструє всі операції зі списком.

#### 2.2.3. Структура програми

```text
lab15/
├── Doxyfile
├── Makefile
├── assets/
│   └── input.txt
├── doc/
│   └── lab15.md
├── src/
│   ├── entity.c
│   ├── entity.h
│   ├── list.c
│   ├── list.h
│   └── main.c
└── test/
    └── test.c
```

### 2.3. Опис та реалізація функцій

#### 2.3.1. Функція list_push

**Призначення:** Додає вакансію у кінець списку. При переповненні подвоює ємність.

**Код:**
```c
int list_push(VacancyList *list, const Vacancy *v) {
    if (list->size == list->capacity) {
        int new_cap = list->capacity == 0 ? INIT_CAPACITY : list->capacity * 2;
        Vacancy *new_data = (Vacancy *)realloc(list->data,
                                               (size_t)new_cap * sizeof(Vacancy));
        if (!new_data)
            return 0;
        list->data = new_data;
        list->capacity = new_cap;
    }
    *(list->data + list->size) = *v;
    list->size++;
    return 1;
}
```

#### 2.3.2. Функція list_remove

**Призначення:** Видаляє елемент за індексом, зсуваючи наступні елементи через розіменування покажчиків.

**Код:**
```c
int list_remove(VacancyList *list, int index) {
    if (index < 0 || index >= list->size)
        return 0;
    for (int i = index; i < list->size - 1; i++)
        *(list->data + i) = *(list->data + i + 1);
    list->size--;
    return 1;
}
```

#### 2.3.3. Функція list_filter_exp_and_vacation

**Призначення:** Метод №1 — обирає вакансії з досвідом більше 2 років та оплачуваною відпусткою.

**Код:**
```c
void list_filter_exp_and_vacation(const VacancyList *list, VacancyList *result) {
    for (int i = 0; i < list->size; i++) {
        const Vacancy *v = list->data + i;
        if (v->years_exp > 2 && v->paid_vacation)
            list_push(result, v);
    }
}
```

#### 2.3.4. Функція list_sort_by_salary

**Призначення:** Сортує список за спаданням максимальної зарплати.

**Код:**
```c
void list_sort_by_salary(VacancyList *list) {
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if ((list->data + j)->salary.max > (list->data + i)->salary.max) {
                Vacancy tmp = *(list->data + i);
                *(list->data + i) = *(list->data + j);
                *(list->data + j) = tmp;
            }
        }
    }
}
```

#### 2.3.5. Функція vacancy_read_file

**Призначення:** Зчитує вакансії з файлу рядок за рядком, поля розділені `;`.

**Код:**
```c
int vacancy_read_file(FILE *f, Vacancy *arr, int max) {
    int count = 0;
    char line[512];
    while (count < max && fgets(line, (int)sizeof(line), f)) {
        trim_newline(line);
        if (*line == '\0') continue;
        char type[16], paid[8], pos[64], company[64];
        char exp[8], sal_min[16], sal_max[16], eng[64];
        char extra1[64], extra2[8];
        int n = sscanf(line,
            "%15[^;];%7[^;];%63[^;];%63[^;];%7[^;];%15[^;];%15[^;];%63[^;];%63[^;];%7s",
            type, paid, pos, company, exp, sal_min, sal_max, eng, extra1, extra2);
        if (n < 8) continue;
        /* заповнення полів структури */
        count++;
    }
    return count;
}
```

#### 2.4. Блок-схеми функцій

**Функція list_push**

![](/doc/assets/function-list_push.png)
![](../../doc/assets/function-list_push.png)

**Функція list_remove**

![](/doc/assets/function-list_remove.png)
![](../../doc/assets/function-list_remove.png)

**Функція list_filter_exp_and_vacation**

![](/doc/assets/function-list_filter_exp_and_vacation.png)
![](../../doc/assets/function-list_filter_exp_and_vacation.png)

**Функція list_sort_by_salary**

![](/doc/assets/function-list_sort_by_salary.png)
![](../../doc/assets/function-list_sort_by_salary.png)

**Функція main**

![](/doc/assets/function-main.png)
![](../../doc/assets/function-main.png)

---

## 3. ВАРІАНТИ ВИКОРИСТАННЯ

Програма компілюється за допомогою `make` та запускається з одним аргументом — шляхом до вхідного файлу.

**Компіляція та запуск:**
```bash
make run
```

**Або вручну:**
```bash
./dist/main.bin "./assets/input.txt"
```

**Приклад роботи:**

![](/doc/assets/result.png)
![](../../doc/assets/result.png)

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
- Реалізовано динамічний масив вакансій `VacancyList` з автоматичним розширенням ємності через `realloc`.
- Реалізовано операції: додавання у кінець, видалення за індексом, фільтрація за критерієм, сортування за зарплатою.
- Доступ до елементів масиву здійснюється виключно через розіменування покажчиків (`*(list->data + i)`).
- Вхідні дані зчитуються з файлу, переданого як аргумент програми.
- Проведено перевірку на витоки пам'яті за допомогою `valgrind` (витоків не виявлено).
- Написано 12 модульних тестів з покриттям коду понад 76%.
- Оформлено документацію Doxygen.

Мета роботи досягнута, програма працює коректно та відповідає поставленому завданню.
