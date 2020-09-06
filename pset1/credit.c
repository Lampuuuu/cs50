#include <cs50.h>
#include <stdio.h>
#include <math.h>

//setting up functions
int luhn(long a);

int length(long b);




int main(void)

{
    //prompting credit card number
    long num = get_long("Number: ");
    int checksum = luhn(num);

    int lgth = length(num);

    //creating start values for checking

    //first digit
    int start1 = num / pow(10, lgth - 1);

    //first 2 digits
    int start10 = num / pow(10, lgth - 2);

    {
        //checking for valid checksum
        if (checksum % 10 == 0)
        {
            //checking the individual conditions for the differing card types
            if (lgth == 15 && (start10 == 34 || start10 == 37))
            {
                printf("AMEX\n");
            }
            else if ((lgth == 16) && (50 < start10) && (56 > start10))
            {
                printf("MASTERCARD\n");
            }
            else if ((lgth == 13 || lgth == 16) && start1 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
}

//setting up variables for Luhn's algorithm
int h1 = 0;     //1st half
int h2 = 0;     //2nd half
int c;

//executing luhn's algorithm
int luhn(long a)
{
    for (long i = 100; i < 10 * a;
         i *= 100)                          //counting variable starts at 100 and gets multiplied by 100 until it is more than 10* input long
    {                                       //this for loop is for filtering the even integers starting by 2nd to last. So 2nd to last, 4th to last etc.
        c = ((a % i) / (i / 10)) * 2;       //variable c is the remainder of the input long divided by i divided by 10
                                            //all of that multiplied by 2
                                            //the result is the exact integer at a certain spot in the long number
        if (c >= 10)
        {
            h1 = h1 + c / 10 + c % 10;      //if the multiplied result is > 10 the single integers get added
        }
        else
        {
            h1 = h1 + c;                    //else the result just gets added
        }
    }
    for (long j = 10; j < 10 * a; j *= 100) //this for loop is for the odd integers starting from last to 3rd to last etc.
    {
        h2 = h2 + ((a % j) / (j / 10));     //same calculation just moved by one power of 10
    }
    int cs = h1 + h2;                       //add the halves up and return them
    return cs;
}

//setting up the length function
long ee = 2;
int lh = 0;
//executing the length function
int length(long b)
{
    for (long cc = 10; ee >= 1; cc *= 10)   //until the countig variable is bigger than the input
    {
                                            //the for loop is executed and the lh counter increases by 1
        lh += 1;
        ee = b / cc;
    }
    return lh;                              //lh counter is returned
}
