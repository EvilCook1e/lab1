#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>
// Веса римских символов
int weight(char c)
{
    switch (c) 
    {
    case 'M': return 7;
    case 'D': return 6;
    case 'C': return 5;
    case 'L': return 4;
    case 'X': return 3;
    case 'V': return 2;
    case 'I': return 1;
    default: return 0;
    }
}

// Проверка римского числа
int is_valid_roman(const char* roman)
{
    int len = strlen(roman);
    if (len == 0) return 0;

    for (int i = 0; i < len; i++)
    {
        char c = toupper(roman[i]);
        if (!strchr("MDCLXVI", c)) return 0;
    }

    // Создаем строку в верхнем регистре для проверок
    char upper[100];
    strcpy(upper, roman);
    for (int i = 0; i < len; i++) {
        upper[i] = toupper(upper[i]);
    }

    //Проверка на повторение символов V, L, D
    for (int i = 0; i < len - 1; i++)
    {
        if ((upper[i] == 'V' || upper[i] == 'L' || upper[i] == 'D') &&
            upper[i] == upper[i + 1])
        {
            return 0; // Найдено VV, LL или DD
        }
    }

    //Проверка на более чем 3 одинаковых символа I, X, C, M подряд
    for (int i = 0; i < len - 3; i++) 
    {
        if ((upper[i] == 'I' || upper[i] == 'X' ||
            upper[i] == 'C' || upper[i] == 'M') &&
            upper[i] == upper[i + 1] &&
            upper[i] == upper[i + 2] &&
            upper[i] == upper[i + 3]) 
        {
            return 0;
        }
    }

    //Проверка правил вычитания
    const char* valid_subtractions[] = { "IV", "IX", "XL", "XC", "CD", "CM" };
    int was_subtraction = 0; // флаг

    for (int i = 0; i < len - 1; i++)
    {
        if (weight(upper[i]) < weight(upper[i + 1])) // найдено вычитание
        {
            char pair[3] = { upper[i], upper[i + 1], '\0' };
            int valid = 0; //флаг

            // Проверяем, является ли комбинация разрешенным вычитанием
            for (int j = 0; j < 6; j++)
            {
                if (strcmp(pair, valid_subtractions[j]) == 0)
                {
                    valid = 1;
                    break;
                }
            }
            if (!valid) return 0;
            if (i > 0 && upper[i - 1] == upper[i])
            {
                return 0;
            }

            i++;
        }
    }

    return 1;
}
//Развернуть римское число (без вычитания)
char* expand_roman(const char* roman) 
{
    char* expanded = (char*)malloc(1000 * sizeof(char));
    expanded[0] = '\0';

    int i = 0;
    while (roman[i] != '\0')
    {
        char current = toupper(roman[i]);
        char next = toupper(roman[i + 1]);

        if (current == 'I' && next == 'V')
        {
            strcat(expanded, "IIII");
            i += 2;
        }
        else if (current == 'I' && next == 'X')
        {
            strcat(expanded, "VIIII");
            i += 2;
        }
        else if (current == 'X' && next == 'L')
        {
            strcat(expanded, "XXXX");
            i += 2;
        }
        else if (current == 'X' && next == 'C')
        {
            strcat(expanded, "LXXXX");
            i += 2;
        }
        else if (current == 'C' && next == 'D')
        {
            strcat(expanded, "CCCC");
            i += 2;
        }
        else if (current == 'C' && next == 'M')
        {
            strcat(expanded, "DCCCC");
            i += 2;
        }
        else
        {
            char temp[2] = { current, '\0' };
            strcat(expanded, temp);
            i++;
        }
    }

    return expanded;
}

// Сортировка символов по убыванию веса
void sort_roman(char* str)
{
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = i + 1; j < len; j++)
        {
            if (weight(str[i]) < weight(str[j]))
            {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}
//Проверка переполнения MMMCMXCIX (3999)
int check_overflow(const char* roman) {
    int consecutive_m = 0;
    int max_consecutive_m = 0;

    for (int i = 0; roman[i]; i++) 
    {
        if (toupper(roman[i]) == 'M') 
        {
            consecutive_m++;
            if (consecutive_m > max_consecutive_m) max_consecutive_m = consecutive_m;
        }
        else consecutive_m = 0;
    }
    return max_consecutive_m <= 3;
}
// Свертка развернутой строки в корректную римскую запись
char* compress_roman(char* expanded)
{
    char* result = (char*)malloc(1000 * sizeof(char));
    strcpy(result, expanded);

    char* patterns[12][2] =
    {
        {"IIIII", "V"},
        {"VV", "X"},
        {"XXXXX", "L"},
        {"LL", "C"},
        {"CCCCC", "D"},
        {"DD", "M"},
        {"VIIII", "IX"},
        {"IIII", "IV"},
        {"LXXXX", "XC"},
        {"XXXX", "XL"},
        {"DCCCC", "CM"},
        {"CCCC", "CD"}
    };

    int flag;
    do {
        flag = 0;
        for (int i = 0; i < 12; i++)
        {
            char* pos = strstr(result, patterns[i][0]);
            if (pos != NULL)
            {
                char temp[1000];
                strcpy(temp, result);
                int index = pos - result;
                temp[index] = '\0';
                strcat(temp, patterns[i][1]);
                strcat(temp, pos + strlen(patterns[i][0]));
                strcpy(result, temp);
                flag = 1;
                break;
            }
        }
    } while (flag);

    return result;
}

// Главная функция
int main()
{
    SetConsoleOutputCP(65001);
    char roman1[100], roman2[100];
    printf("Программа для счёта суммы двух римских чисел. При вводе чисел убедитесь, что они соответствуют правилам римской системы счисления.\n");
    printf("Введите первое римское число: ");
    scanf("%99s", roman1);
    printf("Введите второе римское число: ");
    scanf("%99s", roman2);

    // Проверка корректности первого числа
    if (!is_valid_roman(roman1)) {
        printf("Ошибка: некорректное римское число '%s'\n", roman1);
        return 1;
    }

    // Проверка корректности второго числа
    if (!is_valid_roman(roman2)) {
        printf("Ошибка: некорректное римское число '%s'\n", roman2);
        return 1;
    }

    // Проверка на переполнение для каждого числа
    if (!check_overflow(roman1)) {
        printf("Ошибка: число '%s' превышает 3999\n", roman1);
        return 1;
    }

    if (!check_overflow(roman2)) {
        printf("Ошибка: число '%s' превышает 3999\n", roman2);
        return 1;
    }

    // Развертывание
    char* expanded1 = expand_roman(roman1);
    char* expanded2 = expand_roman(roman2);

    // Объединение
    char combined[2000];
    strcpy(combined, expanded1);
    strcat(combined, expanded2);

    // Сортировка
    sort_roman(combined);

    // Свертка
    char* result = compress_roman(combined);

    // Проверка результата на переполнение
    if (!check_overflow(result)) {
        printf("Ошибка: сумма '%s' + '%s' превышает 3999\n", roman1, roman2);
        free(expanded1);
        free(expanded2);
        free(result);
        return 1;
    }

    // Вывод результата
    printf("Результат: %s + %s = %s\n", roman1, roman2, result);

    // Освобождение памяти
    free(expanded1);
    free(expanded2);
    free(result);

    return 0;
}