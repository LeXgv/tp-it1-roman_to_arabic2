// Copyright [2019] <Copyright Aleksey>

/*
Правила формирования римских Цифр
I = 1
V = 5
X=10
L=50
C=100
D=500
M=1000
Макимальное число повторений одной цифры друг за другом - 3
Если меньшая цифра стоит из большей, то она вычитается из большей.
Пример:
MMMCMXCIX = (начнем ситать справо на лево) = 10-1+100-10+1000-100+1000+
							+1000+1000=3999
MMMDDDCCCLLLXXXVVVIII = 3+15+30+150+300+1500+3000 = 4998 - это неверно
MMMM - ошибка записи числа
Правило вычитания:
IV = 4
IX = 9
XL = 40
XC = 90
CD = 400
CM = 900
Другие комбинации недопустимы
Пример:
Верно: 99 -> XCIX
Неверно 99 -> IC
*/

#include <stdio.h>
#include <string.h>

struct roman_arabic{
    char roman;
    int arabic;
};


//функция преобразующая римскую цифру в арабскую
int getArabic(struct roman_arabic *arr, const  char *roman) {
    for (int i = 0; i < 7; i++) {
        if (arr[i].roman == *roman)
            return arr[i].arabic;
    }
    return 0;
}

int convert_roman_to_decimal(const char roman_str[], int *result) {
    struct roman_arabic value[7] = { {'I', 1}, {'V', 5}, {'X', 10},
                                  {'L', 50}, {'C', 100}, {'D', 500},
                                                        {'M', 1000}};
    int size = strlen(roman_str);
    // переменная с помощью которой определяет, складывать или вычитать
    int modifier = 1;
    // преыдущее число, если оно больше текущего то меняем моификатор на -1
    int before_digit = 0;
    //если равно, то не трогаем модификатор, если оно меньше текущего, то
    // то меняем модификатор на +1
    // счетчик, цифры не должны повторятья друг за другом более 3 раз
    int counter = 0;
    for (int i = size-1; i >= 0; --i) {
        int arabic_value = getArabic(value, &roman_str[i]);
        if (arabic_value == 0) return -2;  // строка содержит неверный символ
        //нет 3 так как первая встреча в строке не засчитывается
        if ((counter == 2) && (before_digit == arabic_value)) {
            return -1;  //ОШИБКА: Строка содержит неверный формат
        }
            if (before_digit > arabic_value) {
                //проверка правила вычитания
                if (((before_digit == 5) && (arabic_value == 1)) ||
                    ((before_digit == 10) && (arabic_value == 1)) ||
                    ((before_digit == 50) && (arabic_value == 10)) ||
                    ((before_digit == 100) && (arabic_value == 10)) ||
                    ((before_digit == 500) && (arabic_value == 100)) ||
                    ((before_digit == 1000) && (arabic_value == 100))) {
                    modifier = -1;
                    counter = 0;
                } else {
                    return -3;  //нарушено правило вычитания
                }
            } else if (before_digit < arabic_value) {
                modifier = 1;
                counter = 0;
            }
            else  ++counter;  // before_digit == roman_value[*itr]
            *result = *result + modifier * arabic_value;
            before_digit = arabic_value;
        }
    return 0;  //успешное выполнение
}

int test_convert_roman_to_decimal() {
    //чтение csv файла с правильныеми значениями
    //перевода римсих цифр в арабские
    FILE *file = fopen("roman.csv", "r");
    if (file == NULL) {
        puts("Don't open file!\n");
        return -1;
    }
    printf("Test 1:\n");
    char str_buffer[20];
    int number = 0;
    int res = 0;
    int status = 0;
    int fr = 1;
    int test_done = 0;
    while (1) {
        res = 0;
       fr =  fscanf(file, "%d;%s\n", &number, str_buffer);
        if (fr == EOF) break;
        status = convert_roman_to_decimal(str_buffer, &res);
        if (!(status == 0 && res == number)) {
            test_done = -1;
            printf("-- fail!  %s -> %d (%d)\n", str_buffer, res, number);
            break;
        }
    }
    fclose(file);
    if (test_done == 0) {
        printf("-- ok\n");
    }
    //Проверка на некорректный формат
    printf("Test 2:\n");
    status = convert_roman_to_decimal("MMMM", &res);
    if (status != -1) {
        printf("-- fail! [%s] status -> %d (%d) - %s\n " ,
                "MMMM", status, -1, "string format error");
        test_done = -1;
    } else {
        printf("-- ok\n");
    }

    printf("Test 3:\n");
    status = convert_roman_to_decimal("IC", &res);
    if (status != -3) {
        printf("-- fail! [%s] status -> %d (%d) - %s \n",
                "IC", status, -3, "subtraction rule violated");
        test_done = -1;
    } else {
        printf("-- ok\n");
    }

    printf("Test 4:\n");
    status = convert_roman_to_decimal("XX=I", &res);
    if (status != -2) {
        printf("-- fail! [%s] status -> %d (%d) - %s \n" ,
                "IC", status, -2, "subtraction rule violated");
        test_done = -1;
    } else {
        printf("-- ok\n");
    }

    return test_done;
}
int main() {
    return test_convert_roman_to_decimal();
}
