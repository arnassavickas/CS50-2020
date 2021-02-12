#include <stdio.h>
#include <math.h>
#include <cs50.h>

int change(void);

int main(void)
{
    int cents = change();
    int remainder;
    int i = 0; //coin counter
    while (cents >= 25) //25 cents
    {
        cents = cents - 25;
        i++;
    }
    while (cents >= 10) //10 cents
    {
        cents = cents - 10;
        i++;
    }
    while (cents >= 5) //5 cents
    {
        cents = cents - 5;
        i++;
    }
    while (cents >= 1) //1 cents
    {
        cents = cents - 1;
        i++;
    }
    printf("%i\n", i);
    
}

int change(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars <= 0);
    
    int cents = round(dollars * 100);
    return cents;
}