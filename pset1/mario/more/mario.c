//mario more

#include <stdio.h>
#include <cs50.h>

int kalnas(void);

int main(void)
{

    int x = kalnas(); //height

    for (int i = 0; i < x; i++) //sets height ie line
    {
        for (int j = x - i; j > 1; j--) //spaces i less than total height
        {
            printf(" ");
        }
        for (int k = -1; k < i; k++) //# same as i
        {
            printf("#");
        }
        printf("  "); //two spaces
        for (int k = -1; k < i; k++) //again # same as i
        {
            printf("#");
        }
        printf("\n"); //new line
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