#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// setting up the functions I am going to use in this programm.

string cipher(string key, string text);
int check(string key);

// the main counts number of arguments and creates string for them
int main(int argc, string argv[])
{
    // If there is no key the programm will return a faulty value and also remind the user how to use the programm
    if (argc <= 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    // will only work if there are exactly 2 input arguments (the program and the key)
    if (argc == 2)
    {
        // assigning the input key to the variable "key"
        string key = argv[1];
        
        // checking the key with the function "check".
        // "check" will return 0 if the key is a valid key and 1 if it isn't among some text outputs.
        int keycheck = check(key);
        
        // if the key is faulty, the main function will also return a 1 to indicate an error.
        if (keycheck == 1)
        {
            return 1;
        }
        
        // if the key is valid the program will the prompt for a plaintext to cipher and assign it to "input"
        // the input the is ciphered according to the key.
        else if (keycheck == 0)
        {
            string input = get_string("plaintext: ");
            cipher(key, input);
        }
    }
    
    //if there are more than 2 input arguments (program and key), the main will also indicate an error
    //and tell the user how to use it right.
    if (argc > 2)
    {
        printf("Usage: ./substitution key.\n only one key with only characters is allowed.\n");
        return 1;
    }
}


//function check will check the key for:
//      number of characters (only 26 characters are valid)
//      type of characters (only alphabetic characters are valid)
//      duplicates in the key (the key must not contain any duplicates)

int check(string key)
{
    // setting up keycheck integer, which will be returned at the end of the function
    
    int keycheck = 0;
    
    // for-loop counting through each character of the key-string
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        // checking for valid number of characters
        if (strlen(key) != 26)
        {
            printf("key must contain 26 characters\n");
            keycheck = 1;
        }
        
        // checking if there are non-alphabetic characters by their ASCII-code 
        else if (key[i] < 'A' || (key[i] > 'Z' && key[i] < 'a') || key[i] > 'z')
        {
            printf("the key must only contain alphabetic characters!\n");
            keycheck = 1;
            break;
        }
        
        // checking for repeated character by creating another for loop, which will compare every character
        // to every later character
        for (int j = i + 1, m = strlen(key); j < m; j++)
        {
            
            // will check for exact equal characters, but also for duplicates in upper-/lowercase.
            if (key[j] == key[i] || key[j] == key[i] - 32 || key[j] == key[i] + 32)
            {
                printf("key must not contain repeated characters\n");
                keycheck = 1;
                break;
            }
        }
        // if any of the before is true, the value of the variable "keycheck" is changed to "1"
        // if that is the case the key is invalid and a message was put out to inform on how the key
        // is invalid. The for-loop does not need to continue and gets stopped with "break"
        if (keycheck == 1)
        {
            break;
        }
    }
    // keycheck is returned as result of the function
    return keycheck;
}


//funtcion cipher:
//  will check if the input is an alphabetic character and if it is uppercase or lowercase
//  then it will print the ciphered text according to the key

void cipher(string key, string input)
{
    // strings for uppercase and lowercase characters are set up
    string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string abc = "abcdefghijklmnopqrstuvwxyz";

    //setting up the ciphertext with a line, so the loops can do their work afterward
    printf("ciphertext: ");
    
    //for loop for every character of the input text
    for (int i = 0; i < strlen(input); i++)
    {
        //another for-loop for every character of the key
        for (int j = 0; j < strlen(key); j++)
        {
            
            // if the input character is uppercase 
            if (input[i] == ABC[j])
            {
                // checking if the corresponding character in the key also is uppercase
                if (key[j] <= 'Z' && key[j] >= 'A')
                {
                    //correspondeng key-character is printed into the ciphertext.
                    printf("%c", key[j]);
                }
                
                // if the corresponding key-charactir is lowercase
                else
                {
                    //the uppercase equivalent of that character is printed into the ciphertext
                    printf("%c", key[j] - 32);
                }
            }
            
            // same process for the lowercase characters.
            else if (input[i] == abc[j])
            {
                if (key[j] >= 'a' && key[j] <= 'z')
                {
                    printf("%c", key[j]);
                }
                else
                {
                    printf("%c", key[j] + 32);
                }
            }
            
        }
        
        //if the input character is not an alphabetic character it is printed as it is into the ciphertext. 
        if (input[i] < 'A' || (input[i] > 'Z' && input[i] < 'a') || input[i] > 'z')
        {
            printf("%c", input[i]);
        }
    }
    //new line at the end
    printf("\n");
    
}


