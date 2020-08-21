// Grading a text according to the coleman-Liau-index.

// including everything, we need
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

// Setting up the funtions for letter, word and sentence counting
double count_letters(string let);
double count_words(string wor);
double count_sentences(string sent);


int main(void)
{
    // Prompting the text to grade
    string text = get_string("Text: ");

    // Calculating variables to be able to calculate the index
    double L = ((count_letters(text) / count_words(text)) * 100);       // Letters per 100 words
    double S = ((count_sentences(text) / count_words(text)) * 100);     // Sentences per 100 words

    // Calculating the index and rounding it to get a clear grade.
    double index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // The grade should be in between 1 and 16, if not it will be expressed in that way.
    if (index < 1)
    {
        printf("Before Grade 1");
    }
    else if (index > 16)
    {
        printf("Grade 16+");
    }
    else
    {
        printf("Grade %i", grade);
    }
    printf("\n");
}

// Function to get the exact number of letters in the text
int lnum;
double count_letters(string let)
{
    lnum = 0;

    // For loop to go through each character of the text.
    for (int i = 0, n = strlen(let); i < n; i++)
    {
        // If clauses to check specifically for the decimal ASCII code of the letters
        // If they match, the number counter increases by one
        if (let[i] >= 'a' && let[i] <= 'z')
        {
            lnum += 1;
        }
        else if (let[i] >= 'A' && let[i] <= 'Z')
        {
            lnum += 1;
        }
    }
    return lnum;
}


// Function to get the exact number of words in the text.
int wnum;
double count_words(string wor)
{
    // If the text by chance starts with an empty space, the word count accounts for it.
    // Else the function counts the words by counting spaces. Wrongly formatted texts could potentially distort
    // the word count.
    if (wor[0] != ' ')
    {
        wnum = 1;
    }
    else
    {
        wnum = 0;
    }

    // For loop again going through the entirety of the text character by character
    for (int i = 0, n = strlen(wor); i < n; i++)
    {
        // For every empty space the word count increases by one, starting at 1, because there is generally one
        // more word than empty space in a text
        if (wor[i] == ' ')
        {
            wnum += 1;
        }
    }
    return wnum;
}

// Function to count sentences
int snum;
double count_sentences(string sent)
{
    snum = 0;
    // Again going through the text character by character
    for (int i = 0, n = strlen(sent); i < n; i++)
    {
        // For everyt ".", "?" and "!" the sentence count increases by one.
        // This does NOT account for phrases like Mr. or abbreviations.
        // Therefore the sentence counter is not 100% accurate.
        if (sent[i] == '.' || sent[i] == '?' || sent[i] == '!')
        {
            snum += 1;
        }
    }
    return snum;
}