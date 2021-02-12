//mario less

#include <stdio.h>
#include <cs50.h>

int kalnas(void);

int main(void)
{

    int x = kalnas(); //height and width
//    int i; //height ie lines
//    int j; // width ie characters in line
    for (int i = 0; i < x; i++)
    {
        for (int j = i + 1; x > j; j++)
        {
            printf(" ");
        }
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}

//kalnas production
int kalnas(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
    return n;
}