#include <stdio.h>
#include <cs50.h>
#include <math.h>

int i, j, length;
int starting(long nr);
int main(void)
{
    long nr = get_long("Number: "); //takes a number
    length = starting(nr);
    printf("Length %i\n", length);
}

int starting(long nr)
{
    for (i = 1; nr > j; i++)
    {
        j = pow(10, i + 1);
    }
    return i;
}

