#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Cannot read file\n");
        return 1;
    }
    //required to sprintf to work
    char *filename = malloc(3 * sizeof(int));
    unsigned char buffer[512];
    int check;
    int start;
    //checks for jpeg start
    do
    {
        check = fread(buffer, sizeof(BYTE), 512, f);
        printf("%i %i %i %i\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    while (buffer[0] != 0xff && buffer[1] != 0xd8 && buffer[2] != 0xff && ((buffer[3] & 0xf0) != 0xe0));
    int i = -1;
    do
    {
        i++;
        //makes filename numbered in 000
        sprintf(filename, "%03i.jpg", i);
        FILE *img = fopen(filename, "w");
        fwrite(buffer, sizeof(BYTE), check, img);
        do
        {
            //check is for checking how many bytes were read
            check = fread(buffer, sizeof(BYTE), 512, f);
            printf("Check %i\n", check);
            printf("%i %i %i %i\n", buffer[0], buffer[1], buffer[2], buffer[3]);
            //boolean to check if it was a start of a jpeg
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
            {
                start = 1;
            }
            else
            {
                start = 0;
            }            
            //if it was a start of a jpeg write is skipped and loop repeats where newly found start is written
            if (start == 0)
            {
                fwrite(buffer, sizeof(BYTE), check, img);
            }
        }
        while (start == 0 && check == 512);
        //close file
        fclose(img);
    }
    while (check == 512);
    //dump filename memory
    free(filename);
}
