#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

//define node
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//number of hashes in hashtable (1 for each letter in the alphabet)
const unsigned int N = 26;

//number of words in the largest dictionary (Oxford has about 500,000 so that's what we'll take here but can be modified)
const unsigned int A = 500000;


// Hash tables for words found and not found in dictionary
// all words, that were found in the dictionary are stored here:
node *found[N];

//all words that were not found in the dictionary are stored here:
node *notfound[N];


//string array for each word in the dictionary
char dictArray[A][LENGTH + 1];


//initializing integers for binary search.
int dictMAX = 0;
int dictMIN = 0;






bool binary_search(const char *searchword, int currMIN, int currMAX);

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    //initialzie temporary nodes
    node *tmp = NULL;
    node *tmpnot = NULL;
    node *temp_wordnode = NULL;


    //allocate memory for our searchword + \0 terminator at the end
    char *searchword = malloc(sizeof(char) * (strlen(word) + 1));
    if (searchword == NULL)
    {
        printf("That's not good\n");
        return false;
    }

    //make the searchword the passed down word, but all lowercase
    for (int i = 0; i < strlen(word); i++)
    {
        searchword[i] = tolower(word[i]);
    }
    //end it with \0 terminator.
    searchword[strlen(word)] = '\0';


    // getting the index to map our word later.
    int hashindex = searchword[0] - 97;

    //search the found linked list of starting letter first
    tmp = found[hashindex];
    if (tmp != NULL)
    {
        //go through the linked list, and everytime you go to a new node
        //compare the searchword with the word stored in that node
        while (tmp->next != NULL)
        {
            //word is found: space can be freed and true is returned.
            if (strcmp(tmp->word, searchword) == 0)
            {
                free(searchword);
                return true;
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }

    //now search corresponding notfound linked list
    tmpnot = notfound[hashindex];
    if (tmpnot != NULL)
    {
        //go through list and compare every time.
        while (tmpnot->next != NULL)
        {
            //word not found once already? return false.
            if (strcmp(searchword, tmpnot->word) == 0)
            {
                free(searchword);
                return false;
            }

            tmpnot = tmpnot->next;
        }
    }

    //word is in neither linked list?
    //binary search in dictionary.
    //will return true, if the word is in the dictionary and false if not
    bool binsearch = binary_search(searchword, dictMIN, dictMAX);


    // searchword was found in dictionary
    if (binsearch)
    {
        //allocate memory
        temp_wordnode = malloc(sizeof(node));
        if (temp_wordnode == NULL)
        {
            printf("ERROR MEMORY NOT ALLOCATED\n");
        }

        //copy string into node and NULL node->next.
        strcpy(temp_wordnode->word, searchword);
        temp_wordnode->next = NULL;


        // if the linked list is empty
        if (tmp == NULL)
        {
            //put at node the start of linked list
            found[hashindex] = temp_wordnode;
        }
        else
        {
            //go to end of that linked list (tmp should still be ther, but lets be safe)
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }

            // this node is now at the end of linked list.
            tmp->next = temp_wordnode;
        }

        //free memory of searchword and return true.
        free(searchword);
        return true;
    }

    //searchword is not in dictionary
    else if (!binsearch)
    {
        //allocate memory
        temp_wordnode = malloc(sizeof(node));
        if (temp_wordnode == NULL)
        {
            printf("ERROR MEMORY NOT ALLOCATED\n");
        }

        //copy string into node and NULL node->next
        strcpy(temp_wordnode->word, searchword);
        temp_wordnode->next = NULL;

        //if linked list is still empty
        if (tmpnot == NULL)
        {
            //start of linked list with this
            notfound[hashindex] = temp_wordnode;
        }
        else
        {
            //get to the end
            while (tmpnot->next != NULL)
            {
                tmpnot = tmpnot->next;
            }

            //put at end of linked list
            tmpnot->next = temp_wordnode;
        }

        //free memory allocated for searchword
        free(searchword);
        return false;
    }

    // just in case none of the above happened, which it should have
    // free memory and return false
    free(searchword);
    return false;
}


//recursive binary search algorthim in dictionary
bool binary_search(const char *searchword, int currMIN, int currMAX)
{
    //initiate the range of current search
    int range = currMAX - currMIN;

    //go into the middle of current range
    int position = currMIN + (range / 2);

    //case 1: we found our searchword:
    if (strcmp(searchword, dictArray[position]) == 0)
    {
        return true;
    }

    //case 2: we went through the entire dictionary and the searchword isn't in it
    else if (searchword != dictArray[position] && range <= 1)
    {
        return false;
    }

    //case 3: the searched word is earlier in the alphabet, than the word at current position
    else if (strcmp(searchword, dictArray[position]) < 0)
    {
        //recall yourself with new borders (from currMIN to position).
        return binary_search(searchword, currMIN, position);
    }

    //case 4: the searched word is later in the alphabet, than the word at current position
    else if (strcmp(searchword, dictArray[position]) > 0)
    {
        //recall yourself with new borders (from position to currMAX)
        return binary_search(searchword, position, currMAX);
    }
    return false;
}





// i didn't need this.
unsigned int hash(const char *word)
{
    return 0;
}





// loads dictionary into a prestacked array.
bool load(const char *dictionary)
{
    //open the dictionary file and assign it the pointer dict
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("could not read %s\n", dictionary);
        return false;
    }

    //initialize idices needed to fill the dictArray
    int index = 0;
    int wordcount = 0;

    //fill dictArray
    for (int c = fgetc(dict); c != EOF; c = fgetc(dict))
    {
        // stop in case of stack overflow
        if (wordcount >= A)
        {
            printf("NOT ENOUGH MEMORY IN STACK");
            return false;
        }

        //alphabetical character? put it into the array at current word
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            dictArray[wordcount][index] = c;
            index++;
        }

        // found end of a word
        else if (index > 0)
        {
            //add 0-terminator at end of word
            dictArray[wordcount][index] = '\0';

            //word is now finished and we can go on to the next

            //increase wordcount
            wordcount++;

            //zero back word index
            index = 0;
        }
    }
    //set highest value of dict array to dictMAX
    dictMAX = wordcount;


    //NULL down our hash tables.
    //we only need this in check, but since this function only gets called once
    //and we want to NULL them down at the beginning, here is a good spot.
    for (int i = 0; i < N; i++)
    {
        found[i] = NULL;
        notfound[i] = NULL;
    }

    //if we reached the end of the file, close dict and return true
    //indicating the entire dictionary is loaded.
    if (fgetc(dict) == EOF)
    {
        fclose(dict);
        return true;
    }

    // if by some chance this point is reached, return false
    return false;
}





// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //we saved our size in dictMAX
    return dictMAX;
}







// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //free both hashtables by going through each linked list
    for (int i = 0; i < N; i++)
    {
        while (found[i] != NULL)
        {
            node *temp = found[i]->next;
            free(found[i]);
            found[i] = temp;
        }

        while (notfound[i] != NULL)
        {
            node *temp = notfound[i]->next;
            free(notfound[i]);
            notfound[i] = temp;
        }
    }
    return true;
}
