// Implements a dictionary's functionality
#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table, in this case i did choose
//18277 because its the right number to store considering the first 3 letters of each word ((26*26)+26*26+25)=18277);
const unsigned int N = 18277;

// Hash table with 18277 positions
node *table[N];

//a counter to keep track of the numbers of words in a dictionary
int n_size = 0;

//function to free every memory allocated;
void free_malloc(node *n);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO - index to every word based in the hash function and a pointer to this index on the table
    int index = hash(word);
    node *ptr = table[index];
    //will compare the word with the dictionary words until NULL is reached
    if (ptr != NULL)
    {
        while (ptr->next != NULL)
        {
            if (strcasecmp(word, ptr->word) == 0)
            {
                return true;
            }
            ptr = ptr->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function - if the second word indeed exist, use the first 2 letters
    //to generate the index number, otherwise, if there´s only 1 letter, use only the first letter to generate the index;
    if (word[1] != '\0' && word[2] != '\0')
    {
        return (((((toupper(word[0]) - 'A') + 1) * 26) * 26) + (toupper(word[1]) - 'A' + 1) * 26) + toupper(word[2]) - 'A';
    }
    if (word[1] != '\0')
    {
        return (((toupper(word[0]) - 'A') + 1) * 26) + toupper(word[1]) - 'A';
    }
    else
    {
        return toupper(word[0]) - 'A';
    }
}

// Loads dictionary into memory, returning true if successful, else false;
bool load(const char *dictionary)
{
    // TODO - opening the file in read mode;
    FILE *infile = fopen(dictionary, "r");
    //checking if the file was successfully opened;
    if (infile == NULL)
    {
        printf("Error opening file!\n");
        return false;
    }
    //a provisory buffer to store the words;
    char buffer[LENGTH + 1];
    //while fscanf retunrs a valid word from infile, store it on buffer;
    while (fscanf(infile, "%s", buffer) != EOF)
    {
        //create a index to the word stored in buffer; counts the numbers of words in the file;
        int index = hash(buffer);
        n_size++;
        //will allocate memory for every node and pointer;
        if (table[index] == NULL)
        {
            table[index] = malloc(sizeof(node));

            if (table[index] == NULL)
            {
                printf("Can't allocate memory for it.\n");
                return false;
            }
            //set the pointer next to NULL;
            table[index]->next = NULL;

            node *ptr = malloc(sizeof(node));

            if (ptr == NULL)
            {
                printf("Can't allocate memory for it.\n");
                return false;
            }
            //will copy every buffer word to the node were the pointer ptr->word is pointing to,
            //point the next pointer of node to where the table index is pointing and point the table index to where ptr
            //is pointing to;
            strcpy(ptr->word, buffer);
            ptr->next = table[index];

            table[index] = ptr;
        }
        //if the first node of the index aledy exists, will allocate memory for every node pointer;
        else if (table[index] != NULL)
        {
            node *ptr = malloc(sizeof(node));

            if (ptr == NULL)
            {
                printf("Can't allocate memory for it.\n");
                return false;
            }
            //will copy every buffer word to the node were the pointer ptr->word is pointing to,
            //point the next pointer of node to where the table index is pointing and point the table index to where ptr
            //is pointing to;
            strcpy(ptr->word, buffer);
            ptr->next = table[index];

            table[index] = ptr;
        }
    }
    //close the file and return true if its successful
    fclose(infile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (n_size > 0)
    {
        return n_size;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int index = 0;
    //while index is less then N, will check if the table index has value, if it has,
    //will point to this location through a node pointer and call the function free_malloc
    //to free the memory allocated for it, after that, will iterate ondex to the next location on the table;
    while (index != N)
    {
        if (table[index] != NULL)
        {
            node *ptr = table[index];

            free_malloc(ptr);
            index++;
        }
        else
        {
            index++;
        }
    }
    //if index has the same number of N, return true;
    if (index == N)
    {
        return true;
    }

    return false;
}
//recursive function to free every memory allocated to the dictionary;
void free_malloc(node *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    free_malloc(ptr->next);

    free(ptr);
}