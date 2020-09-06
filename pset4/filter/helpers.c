#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
int rgbavg(int a, int b, int c);
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            int rgbiavg = rgbavg(image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
            image[i][j].rgbtRed = rgbiavg;
            image[i][j].rgbtGreen = rgbiavg;
            image[i][j].rgbtBlue = rgbiavg;
        }
    }
    return;
}
int rgbavg(int a, int b, int c)
{
    float aa = a;
    float bb = b;
    float cc = c;
    int avg = round((aa + bb + cc)/3);
    return avg;
}




// Reflect image horizontally

void swappixels(RGBTRIPLE *a, RGBTRIPLE *b);
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
      for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w/2; j++)
        {
            swappixels(&image[i][j], &image[i][width - j - 1]);
        }
    }
}
void swappixels(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}







// Blur image

// function with i and j as inputs

void floorceil(int i, int max, int *ifloor, int *iceil);

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //collecting the initial values of th picture
    RGBTRIPLE oldvalues[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldvalues[i][j] = image[i][j];
        }
    }
    //setting up variables
    int afloor, aceil;
    int bfloor, bceil;
    double rsum, gsum, bsum;
    float rnow, gnow, bnow;

    //going through each pixel
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            // calculating the floor and ceiling for the blurwindow
            floorceil(i, height - 1, &afloor, &aceil);
            floorceil(j, width - 1, &bfloor, &bceil);

            //more variables, that need to be ==0 for each pixel
            int cc = 0;
            rsum = gsum = bsum = 0;

            // going through the 3x3 or less window
            for (int a = afloor; a <= aceil; a++)
            {
                for (int b = bfloor; b <= bceil; b++)
                {
                    //collecting and summing up the initial values
                    rnow = oldvalues[a][b].rgbtRed;
                    gnow = oldvalues[a][b].rgbtGreen;
                    bnow = oldvalues[a][b].rgbtBlue;
                    rsum += rnow;
                    gsum += gnow;
                    bsum += bnow;
                    cc += 1;

                }
            }
            int redavg = round(rsum / cc);
            int greenavg = round(gsum / cc);
            int blueavg = round(bsum / cc);
            image[i][j].rgbtRed = redavg;
            image[i][j].rgbtGreen = greenavg;
            image[i][j].rgbtBlue = blueavg;

        }
    }
    return;
}
void floorceil(int i, int max, int *ifloor, int *iceil)
{
    if (i == 0)
    {
        *ifloor = 0;
    }
    else
    {
        *ifloor = i - 1;
    }

    if (i == max)
    {
        *iceil = i;
    }
    else
    {
        *iceil = i + 1;
    }
}


// Detect edges
int lightcheck(int G);

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // setting up sobel operators
    int Gxarr[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gyarr[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // copying the old values into new array with 2 more rows and cols
    RGBTRIPLE oldvalues[height + 2][width + 2];
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            oldvalues[i][j] = image[i][j];
        }
    }

    //initializing Gx and Gy Variables for each color
    double GxRed, GxGrn, GxBlu;
    double GyRed, GyGrn, GyBlu;

    // each pixel in the grid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Zeroing down the counting values each pixel
            GxRed = GxGrn = GxBlu = GyRed = GyGrn = GyBlu = 0;
            int cc = 0;
            // creating the 9x9 grid
            for (int a = -1; a < 2; a++)
            {
                 for (int b = -1; b < 2; b++)
                {

                    // getting each old value
                    int ovred = oldvalues[i + 1 + a][j + 1 + b].rgbtRed;
                    int ovgrn = oldvalues[i + 1 + a][j + 1 + b].rgbtGreen;
                    int ovblu = oldvalues[i + 1 + a][j + 1 + b].rgbtBlue;

                    // Gx Values
                    GxRed += ovred * Gxarr[cc];
                    GxGrn += ovgrn * Gxarr[cc];
                    GxBlu += ovblu * Gxarr[cc];

                    //Gy Values
                    GyRed += ovred * Gyarr[cc];
                    GyGrn += ovgrn * Gyarr[cc];
                    GyBlu += ovblu * Gyarr[cc];
                    cc ++;

                }
            }
            int GRed = round(sqrt(pow(GxRed, 2) + pow(GyRed, 2)));
            int GGrn = round(sqrt(pow(GxGrn, 2) + pow(GyGrn, 2)));
            int GBlu = round(sqrt(pow(GxBlu, 2) + pow(GyBlu, 2)));

            //check if G-Values are greater than 255
            GRed = lightcheck(GRed);
            GGrn = lightcheck(GGrn);
            GBlu = lightcheck(GBlu);

            image[i][j].rgbtRed = GRed;
            image[i][j].rgbtGreen = GGrn;
            image[i][j].rgbtBlue = GBlu;
        }

    }

    return;
}

int lightcheck(int G)
{
    if (G > 255)
    {
        G = 255;
    }
    return G;
}
