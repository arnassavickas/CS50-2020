#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //average of RGB channels
            float average = roundf(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int temp_blue[height][width], temp_green[height][width], temp_red[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_blue[i][j] = image[i][j].rgbtBlue; //copies image
            temp_green[i][j] = image[i][j].rgbtGreen;
            temp_red[i][j] = image[i][j].rgbtRed;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, x = width - 1; j < width; j++) //rewrites original but reflected, starting from the right
        {
            image[i][j].rgbtBlue = temp_blue[i][x - j];
            image[i][j].rgbtGreen = temp_green[i][x - j];
            image[i][j].rgbtRed = temp_red[i][x - j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newimage[height][width];
    float temp_blue[height][width], temp_green[height][width], temp_red[height][width];
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int counter = 0;
            temp_blue[i][j] = 0;
            temp_green[i][j] = 0;
            temp_red[i][j] = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++) //goes around [i][j] pixel and adds every channel
                {
                    if ((i + x) >= 0 && (j + y) >= 0 && (i + x) < height && (j + y) < width) //conditions necessary not to choose pixels out of bounds
                    {
                        temp_blue[i][j] += (float)image[i + x][j + y].rgbtBlue;
                        temp_green[i][j] += (float)image[i + x][j + y].rgbtGreen;
                        temp_red[i][j] += (float)image[i + x][j + y].rgbtRed;
                        counter++; //counter to know how many pixels are added
                    }
                }
            }
            newimage[i][j].rgbtBlue = round(temp_blue[i][j] / counter);  //divides by how many pixels were added
            newimage[i][j].rgbtGreen = round(temp_green[i][j] / counter);
            newimage[i][j].rgbtRed = round(temp_red[i][j] / counter);
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = newimage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = newimage[i][j].rgbtGreen;
            image[i][j].rgbtRed = newimage[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE org[height][width];
    int i, j;
    int counter = 0;
    //kernels
    int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    //copy image
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            org[i][j] = image[i][j];
        }
    }

    double blueX, blueY, greenX, greenY, redX, redY, blue, green, red;

    //goes through every pixel
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            counter = 0; //counter is for choosing which kernel pixel to use in an 3x3 array
            blueX = 0;
            blueY = 0;
            greenX = 0;
            greenY = 0;
            redX = 0;
            redY = 0;
            for (int x = -1; x <= 1; x++) //goes through a 3x3 pixel array around [i][j] pixel and multiplies to kernel
            {
                for (int y = -1; y <= 1; y++)
                {
                    if ((i + x) >= 0 && (j + y) >= 0 && (i + x) < height && (j + y) < width)
                    {
                        blueX += org[i + x][j + y].rgbtBlue * Gx[counter];
                        blueY += org[i + x][j + y].rgbtBlue * Gy[counter];
                        greenX += org[i + x][j + y].rgbtGreen * Gx[counter];
                        greenY += org[i + x][j + y].rgbtGreen * Gy[counter];
                        redX += org[i + x][j + y].rgbtRed * Gx[counter];
                        redY += org[i + x][j + y].rgbtRed * Gy[counter];
                        counter++; //goes to next kernel element
                    }
                    else if ((i + x) < 0 || (j + y) < 0 || (i + x) >= height || (j + y) >= width) //edge and corner conditions
                    {
                        blueX += 0 * Gx[counter];
                        blueY += 0 * Gy[counter];
                        greenX += 0 * Gx[counter];
                        greenY += 0 * Gy[counter];
                        redX += 0 * Gx[counter];
                        redY += 0 * Gy[counter];
                        counter++;                        
                    }
                }
            }
            //adding Gx and Gy accoring to the formula
            blue = round(sqrtl(pow(blueX, 2) + pow(blueY, 2)));
            green = round(sqrtl(pow(greenX, 2) + pow(greenY, 2)));
            red = round(sqrtl(pow(redX, 2) + pow(redY, 2)));
            //if total is over 255, capping it at max 255
            if (blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = blue;
            }
            if (green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = green;
            }
            if (red > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = red;
            }
        }
    }
    return;
}
