#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    int l = 0; //letters
    int w = 1; //words
    int s = 0; //sentences
    
    string text = get_string("Text: "); //input the text
    
    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 92 && text[i] < 123))
        {
            l++;
        }
        if (text[i] == 32)
        {
            w++;
        }
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            s++;
        }
    }
    
    float index = 0.0588 * (l * 100) / w - 0.296 * (s * 100) / w - 15.8; //calculates the level
    int grade = roundf(index);
    
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}