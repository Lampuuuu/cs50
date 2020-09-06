//Builds a pyramid of size prompted by the user


#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    //prompting size of the pyramid
    do
    {
        n = get_int("Height: ");
    }

    //only continuing if the Height is between 1 and 8
    while (1 > n || n > 8);

    // setting the height of the pyramid by creating n amount of new columns concluded in c43
    for (int i = 0; i < n; i++)
    {
        // settomg empty spaces befor the first #
        for (int j = n - i - 1; j > 0; j--)
        {
            printf(" ");
        }

        // setting the first half of # for the pyramid
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        // two empty spaces in the middle
        printf("  ");

        // setting the second half of the pyramid
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }

        printf("\n");
    }
}
