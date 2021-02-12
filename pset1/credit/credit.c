#include <stdio.h>
#include <cs50.h>
#include <math.h>

long ask_number(void);
int sum1(long nr);
int sum2(long nr);
int starting(long nr);
int main(void)
{
    long nr = ask_number();
    if (nr < pow(10, 12) || nr > pow(10, 16))
    {
        printf("INVALID\n");
    }
    else
    {
        int multiplied = sum1(nr);
        int sum = sum2(nr);
        int total = multiplied + sum;
        int first = starting(nr);
        if (total % 10 == 0) //tikrina ar algoritmo paskutinis skaitmuo yra 0
        {
            if (first == 4) //VISA pirmas skaicius 4
            {
                printf("VISA\n");
            }
            else if (first == 34 || first == 37) //AMEX pirmi skaiciai
            {
                printf("AMEX\n");
            }
            else if (first >= 51 && first <= 55) //MASTERCARD pirmi skaiciai
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
}

long ask_number(void) //klausia koks nr, ir paduoda numeri
{res = [int(x) for x in str(num)]
    long i;
    i = get_long("Number: "); //takes a number
    return i;
}

int sum1(long nr) //skaiciuoja kas antro skaiciaus daugyba ir suma
{
    int sum = 0;
    for (int i = 2; i < 17; i = i + 2)
    {
        long a, b, c, d, e, x, y;
        x = pow(10, i);
        y = pow(10, i - 1);
        a = nr % x;
        b = nr % y;
        c = a - b;
        d = c / y;
        e = d * 2;
    
        if (e >= 10) //jeigu gaunasi dvizenklis skaicius, tai padaro is abieju skaitmenu suma
        {
            e = 1 + e % 10;
        }
//    printf("Chosen nummber sum1: %li\n", d); //prints chosen numbers
        sum = sum + e;
    }
//    printf("sum1%i\n", sum);
    return sum;
}

int sum2(long nr) //skaiciuoja kita suma kur be daugybos
{
    int sum = 0;
    for (int i = 1; i < 17; i = i + 2)
    {
        long a, b, c, d, x, y;
        x = pow(10, i);
        y = pow(10, i - 1);
        a = nr % x;
        b = nr % y;
        c = a - b;
        d = c / y;
//    printf("Chosen nummber sum2: %li\n", d); //prints chosen numbers
        sum = sum + d;
    }
//    printf("sum2%i\n", sum);
    return sum;
}

int starting(long nr) //skaiciuoja kokio ilgio skaicius ir paima pirmuosius skaicius
{
    int d;
    int i = 1;
    int length, first, second;
    long a, b, c, x, y;
    long j = 0;
    for (length = 1; nr > j; length++)
    {
        j = pow(10, length + 1);
    }
    x = pow(10, length - 0);
    y = pow(10, length - 1);
    a = nr % x;
    b = nr % y;
    c = a - b;
    first = c / y;
    if (first != 4)
    {
        x = pow(10, length - 1);
        y = pow(10, length - 2);
        a = nr % x;
        b = nr % y;
        c = a - b;
        first = first * 10 + c / y;
    }
    return first;
}