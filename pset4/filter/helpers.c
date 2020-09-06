#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
int rgbavg(int a, int b, int c);
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //going through each pixel
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //averaging out every pixel via function below
            int rgbiavg = rgbavg(image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);

            //adding the averaged (grey) value to the output
            image[i][j].rgbtRed = rgbiavg;
            image[i][j].rgbtGreen = rgbiavg;
            image[i][j].rgbtBlue = rgbiavg;
        }
    }
    return;
}

// funcion takes in 3 ints, converts them to floats to allow rounding and then divides them by three
int rgbavg(int a, int b, int c)
{
    float aa = a;
    float bb = b;
    float cc = c;
    int avg = round((aa + bb + cc) / 3);
    return avg;
}



// Reflect image horizontally
void swappixels(RGBTRIPLE *a, RGBTRIPLE *b);
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //going through half of the pixels
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w / 2; j++)
        {
            //swapping current pixel with corresponding mirrored pixel via funct below.
            swappixels(&image[i][j], &image[i][width - j - 1]);
        }
    }
    return;
}
//swaps to RGBTRIPLE values, by taking in their adresses and repointing them.
void swappixels(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}



// Blur image
void floorceil(int i, int max, int *ifloor, int *iceil);
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copying the original image into the <oldvalues>-array
    RGBTRIPLE oldvalues[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldvalues[i][j] = image[i][j];
        }
    }
    //initialize variables
    int afloor, aceil;
    int bfloor, bceil;
    double rsum, gsum, bsum;
    float rnow, gnow, bnow;

    //going through each pixel
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //determining the size of the blur grid via function below.
            floorceil(i, height - 1, &afloor, &aceil);
            floorceil(j, width - 1, &bfloor, &bceil);

            //initialize counting variable to divide by later
            int cc = 0;

            //zero down sum variables
            rsum = gsum = bsum = 0;

            //create array around current pixel, size is determined by <floorceil>
            for (int a = afloor; a <= aceil; a++)
            {
                for (int b = bfloor; b <= bceil; b++)
                {
                    //get the color values from the copied array
                    rnow = oldvalues[a][b].rgbtRed;
                    gnow = oldvalues[a][b].rgbtGreen;
                    bnow = oldvalues[a][b].rgbtBlue;

                    // add them all up
                    rsum += rnow;
                    gsum += gnow;
                    bsum += bnow;

                    //increase counter by 1(since the size of the grid varies)
                    cc += 1;
                }
            }

            //get the averages by dividing by the counter
            int redavg = round(rsum / cc);
            int greenavg = round(gsum / cc);
            int blueavg = round(bsum / cc);

            //put the averages into the output.
            image[i][j].rgbtRed = redavg;
            image[i][j].rgbtGreen = greenavg;
            image[i][j].rgbtBlue = blueavg;
        }
    }
    return;
}

//determines minimum and maximum values for the blur grid, by taking
//the current value, the max falue and the adresses of floor and ceiling variables
void floorceil(int i, int max, int *ifloor, int *iceil)
{
    //if i matches 0 or the max value, the floor/ceil value is changed.
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
    // arrays for Gx and Gy
    int Gxarr[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gyarr[3][3] =
    {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };


    //RGBTRIPLE array with 2 more rows and columns
    //Border rows and cols are black
    RGBTRIPLE oldvalues[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            //if condition for the borders
            if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
            {
                oldvalues[i][j].rgbtRed = 0;
                oldvalues[i][j].rgbtGreen = 0;
                oldvalues[i][j].rgbtBlue = 0;
            }
            else
            {
                //copy the image values in the middle of the array.
                oldvalues[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
                oldvalues[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
                oldvalues[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
            }
        }
    }

    //initialize variables, needed later
    double GxRed, GxGrn, GxBlu;
    double GyRed, GyGrn, GyBlu;

    // double for loop to go through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //zeroing down variables, that need to be == 0 for every pixel
            GxRed = GxGrn = GxBlu = GyRed = GyGrn = GyBlu = 0;

            //double for loop for the 9x9 grid around the current pixel
            for (int a = 0; a < 3; a++)
            {
                for (int b = 0; b < 3; b++)
                {
                    // getting the orinal values into a variable representing the current grid value
                    int ovred = oldvalues[i + a][j + b].rgbtRed;
                    int ovgrn = oldvalues[i + a][j + b].rgbtGreen;
                    int ovblu = oldvalues[i + a][j + b].rgbtBlue;

                    //summing up the original values multiplied with the corresponding value in the
                    //<Gxarr> and <Gyarr> creating 2 values for each color
                    GxRed += ovred * Gxarr[a][b];
                    GxGrn += ovgrn * Gxarr[a][b];
                    GxBlu += ovblu * Gxarr[a][b];

                    GyRed += ovred * Gyarr[a][b];
                    GyGrn += ovgrn * Gyarr[a][b];
                    GyBlu += ovblu * Gyarr[a][b];
                }
            }

            //merging the X and Y value with given formula and rounding it
            int GRed = round(sqrt(pow(GxRed, 2) + pow(GyRed, 2)));
            int GGrn = round(sqrt(pow(GxGrn, 2) + pow(GyGrn, 2)));
            int GBlu = round(sqrt(pow(GxBlu, 2) + pow(GyBlu, 2)));

            //check if merged value > 255 via <lightcheck>
            GRed = lightcheck(GRed);
            GGrn = lightcheck(GGrn);
            GBlu = lightcheck(GBlu);

            //add value to corresponding spot in the output grid.
            image[i][j].rgbtRed = GRed;
            image[i][j].rgbtGreen = GGrn;
            image[i][j].rgbtBlue = GBlu;
        }
    }
    return;
}

// checks if value is > 255 and tunes it down to 255.
int lightcheck(int G)
{
    if (G > 255)
    {
        G = 255;
    }
    return G;
}
