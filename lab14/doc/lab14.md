<p align="center">
  <strong>Лабораторна робота №14</strong><br>
  <strong>Тема: Структуровані типи даних</strong>
</p>

---

## 1. ВИМОГИ

### 1.1. Розробник

- **Студентка:** Рудь Олександра
- **Група:** КН-925а
- **Статус:** студентка НТУ «Харківський політехнічний інститут»
- **Дата розробки:** 18.05.2026

### 1.2. Загальне завдання

Розробити повноцінні звіти для лабораторної роботи **«Структуровані типи даних»**, що присвячена роботі зі структурами у мові програмування C, у двох форматах:

- **Markdown** (поточний файл `lab14.md`);
- **DOC**-формат, згідно з вимогами **ДСТУ**, з подальшим збереженням у PDF.

При цьому необхідно:

- створити коментарі до коду в стилі **Doxygen**;
- згенерувати HTML-документацію за допомогою Doxygen;
- продемонструвати відсутність витоків пам'яті за допомогою утиліти **valgrind**;
- забезпечити покриття коду модульними тестами (мінімум 50%);
- доступ до елементів масиву здійснювати через розіменування покажчиків;
- передачу об'єктів структури у функції та отримання результатів виконувати виключно за вказівником;
- вхідні дані зчитувати з файлу, переданого як аргумент програми — нічого від користувача не очікувати.

### 1.3. Індивідуальне завдання

**Прикладна галузь: Вакансія**

Поля базового класу:
- наявність оплачуваної відпустки (так / ні);
- позиція (Junior Java Developer, Middle Java Developer, Senior Android Developer, Sales Manager, Project Manager);
- назва компанії;
- кількість років досвіду;
- діапазон заробітної плати (структура з нижньою та верхньою межами);
- необхідний рівень знання англійської (pre-intermediate, intermediate, upper-intermediate, advanced).

**Спадкоємець 1 — Офісна вакансія.** Додаткові поля:
- адреса офісу;
- наявність спортзалу.

**Спадкоємець 2 — Віддалена вакансія.** Додаткові поля:
- бажаний часовий пояс (GMT+2, PT, EST);
- наявність досвіду віддаленої роботи.

**Методи для роботи з колекцією:**
1. Обрати всі вакансії, що потребують більше 2 років досвіду та пропонують оплачувану відпустку.
2. Відсортувати вакансії за назвою компанії та обрати першу вакансію, що потребує англійської рівня Intermediate або вище.
3. Знайти віддалену вакансію з найбільшою можливою заробітною платою.

---

## 2. ОПИС ПРОГРАМИ

### 2.1. Функціональне призначення

Програма призначена для обробки колекції вакансій, зчитаних з текстового файлу.

Основні можливості:
1. Зчитати список офісних та віддалених вакансій з вхідного файлу.
2. Застосувати три методи фільтрації та пошуку до колекції.
3. Вивести результати кожного методу у консоль.
4. Коректно звільнити всю динамічно виділену пам'ять.

**Обмеження на застосування:**
- максимальна кількість вакансій кожного типу — 128;
- рядки полів обмежені 64 символами;
- програма не очікує жодного вводу від користувача під час роботи.

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
} Vacancy;

typedef struct {
    Vacancy base;
    char    office_address[64];
    int     has_gym;
} OfficeVacancy;

typedef struct {
    Vacancy base;
    char    timezone[64];
    int     remote_exp;
} RemoteVacancy;

typedef struct {
    OfficeVacancy office[128];
    RemoteVacancy remote[128];
    int           office_count;
    int           remote_count;
} VacancyCollection;
```

#### 2.2.2. Структура функцій

Основні функції програми (описи відповідають коментарям Doxygen):

- `int str_cmp(const char *a, const char *b)`  
  Порівнює два рядки побайтово через розіменування покажчиків. Повертає від'ємне/0/додатнє значення.

- `int english_level_rank(const char *level)`  
  Повертає числовий рівень англійської: 0 — pre-intermediate, 1 — intermediate, 2 — upper-intermediate, 3 — advanced.

- `void read_vacancies(FILE *f, VacancyCollection *col)`  
  Зчитує вакансії з файлу рядок за рядком. Поля розділені символом `;`. Розрізняє офісні та віддалені вакансії за першим полем.

- `void print_vacancy(const Vacancy *v)`  
  Виводить поля базової вакансії.

- `void print_office_vacancy(const OfficeVacancy *v)`  
  Виводить поля офісної вакансії: базові + адреса та наявність спортзалу.

- `void print_remote_vacancy(const RemoteVacancy *v)`  
  Виводить поля віддаленої вакансії: базові + часовий пояс та досвід remote.

- `void filter_exp_and_vacation(const VacancyCollection *col, Vacancy *result, int *count)`  
  Метод 1: обирає вакансії з досвідом > 2 років та оплачуваною відпусткою.

- `int sort_and_find_intermediate(const VacancyCollection *col, Vacancy *result)`  
  Метод 2: сортує всі вакансії за компанією та повертає першу з рівнем англійської >= Intermediate.

- `int find_remote_max_salary(const VacancyCollection *col, RemoteVacancy *result)`  
  Метод 3: знаходить віддалену вакансію з найбільшою максимальною зарплатою.

- `int main(int argc, char *argv[])`  
  Точка входу. Приймає шлях до файлу як аргумент, зчитує колекцію, виводить результати трьох методів.

#### 2.2.3. Структура програми

```text
lab14/
├── Doxyfile
├── Makefile
├── assets/
│   └── input.txt
├── doc/
│   └── lab14.md
├── src/
│   ├── lib.c
│   ├── lib.h
│   └── main.c
└── test/
    └── test.c
```

### 2.3. Опис та реалізація функцій

#### 2.3.1. Функція filter_exp_and_vacation

**Призначення:** Обирає всі вакансії з досвідом більше 2 років та оплачуваною відпусткою.

**Код:**
```c
void filter_exp_and_vacation(const VacancyCollection *col, Vacancy *result, int *count) {
    *count = 0;
    for (int i = 0; i < col->office_count; i++) {
        const Vacancy *v = &(col->office + i)->base;
        if (v->years_exp > 2 && v->paid_vacation) {
            *(result + *count) = *v;
            (*count)++;
        }
    }
    for (int i = 0; i < col->remote_count; i++) {
        const Vacancy *v = &(col->remote + i)->base;
        if (v->years_exp > 2 && v->paid_vacation) {
            *(result + *count) = *v;
            (*count)++;
        }
    }
}
```

#### 2.3.2. Функція sort_and_find_intermediate

**Призначення:** Сортує всі вакансії за назвою компанії та повертає першу з рівнем англійської Intermediate або вище.

**Код:**
```c
int sort_and_find_intermediate(const VacancyCollection *col, Vacancy *result) {
    int total = col->office_count + col->remote_count;
    if (total == 0)
        return 0;

    Vacancy *arr = (Vacancy *)malloc((size_t)total * sizeof(Vacancy));
    if (!arr)
        return 0;

    collect_all_base(col, arr, &total);
    sort_by_company(arr, total);

    int found = 0;
    for (int i = 0; i < total; i++) {
        if (english_level_rank((arr + i)->english_level) >= 1) {
            *result = *(arr + i);
            found = 1;
            break;
        }
    }
    free(arr);
    return found;
}
```

#### 2.3.3. Функція find_remote_max_salary

**Призначення:** Знаходить віддалену вакансію з найбільшою максимальною зарплатою.

**Код:**
```c
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
```

#### 2.3.4. Функція read_vacancies

**Призначення:** Зчитує вакансії з файлу рядок за рядком. Поля розділені символом `;`.

**Код:**
```c
void read_vacancies(FILE *f, VacancyCollection *col) {
    col->office_count = 0;
    col->remote_count = 0;
    char line[512];
    while (fgets(line, (int)sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0')
            continue;
        char type[16], paid[8], pos[64], company[64];
        char exp[8], sal_min[16], sal_max[16], eng[64];
        char extra1[64], extra2[8];
        int n = sscanf(line,
            "%15[^;];%7[^;];%63[^;];%63[^;];%7[^;];%15[^;];%15[^;];%63[^;];%63[^;];%7s",
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
```

#### 2.3.5. Функція str_cmp

**Призначення:** Порівнює два рядки побайтово через розіменування покажчиків.

**Код:**
```c
int str_cmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
```

#### 2.3.6. Функція english_level_rank

**Призначення:** Повертає числовий рівень англійської для порівняння.

**Код:**
```c
int english_level_rank(const char *level) {
    if (str_cmp(level, "pre-intermediate") == 0)  return 0;
    if (str_cmp(level, "intermediate") == 0)       return 1;
    if (str_cmp(level, "upper-intermediate") == 0) return 2;
    if (str_cmp(level, "advanced") == 0)           return 3;
    return -1;
}
```

#### 2.4. Блок-схеми функцій

**Функція filter_exp_and_vacation**

![](/doc/assets/function-filter_exp_and_vacation.png)
![](../../doc/assets/function-filter_exp_and_vacation.png)

**Функція sort_and_find_intermediate**

![](/doc/assets/function-sort_and_find_intermediate.png)
![](../../doc/assets/function-sort_and_find_intermediate.png)

**Функція find_remote_max_salary**

![](/doc/assets/function-find_remote_max_salary.png)
![](../../doc/assets/function-find_remote_max_salary.png)

**Функція read_vacancies**

![](/doc/assets/function-read_vacancies.png)
![](../../doc/assets/function-read_vacancies.png)

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

---

## ВИСНОВКИ

У ході виконання лабораторної роботи було:
- Реалізовано базову структуру `Vacancy` та два похідних типи: `OfficeVacancy` та `RemoteVacancy`.
- Реалізовано три методи для роботи з колекцією вакансій: фільтрація за досвідом та відпусткою, сортування за компанією з пошуком за рівнем англійської, пошук максимальної зарплати серед віддалених вакансій.
- Передача об'єктів структур у функції та отримання результатів виконується виключно за вказівником.
- Доступ до елементів масивів здійснюється через розіменування покажчиків (`*(arr + i)`).
- Вхідні дані зчитуються з файлу, переданого як аргумент програми.
- Проведено перевірку на витоки пам'яті за допомогою `valgrind` (витоків не виявлено).
- Написано 14 модульних тестів з покриттям коду понад 84%.
- Оформлено документацію Doxygen.

Мета роботи досягнута, програма працює коректно та відповідає поставленому завданню.
