// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
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

// Number of buckets in hash table
////choosing some prime numbers////
const unsigned int N = 6131;

// Hash table
node *table[N];

//dictionary counter
unsigned int dict_counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hval = hash(word);
    node *tmp = table[hval];
    while (tmp != NULL)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
// djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    //open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open\n");
        return false;
    }
    char *word = malloc(LENGTH + 1);
    if (word == NULL)
    {
        return false;
    }
    while (fscanf(dict, "%s", word) != EOF)
    {
        //create a new node for each word
        node *nword = malloc(sizeof(node));
        if (nword == NULL)
        {
            return false;
        }
        //hash word to obtain a hash value
        unsigned int hval;
        hval = hash(word);
        //insert word into a hash table at that location
        
        strcpy(nword->word, word);
        //if this is the start of that hash
        if (table[hval] == NULL)
        {
            nword->next = NULL;
        }
        //if this is an existing hash
        else
        {
            nword->next = table[hval];
        }
        table[hval] = nword;
        //adds counted words
        dict_counter++;
//        free(nword);
    }
    fclose(dict);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
