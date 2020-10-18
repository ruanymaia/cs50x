// Ruany Maia
// 09 Apr 2020
// Problem Set 5 - Speller

// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Initiate dictionary word counter
int word_counter = 0;
int *word_counter_ptr = &word_counter;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // Creates buffer for lowered case words from the text
    char lcase_word[LENGTH + 1];

    for (int i = 0; i < LENGTH + 1; i++)
    {
        // Empties buffer
        lcase_word[i] = 0;

        // Lowercase word char by char
        for (int j = 0, n = strlen(word); j < n; j++)
        {
            lcase_word[j] = tolower(word[j]);
        }
    }

    // Hashes word
    int index = hash(lcase_word);

    // Assigns temporary node to the first node of singly linked list
    node *temp = table[index];

    // While singly linked list is not NULL
    while (temp != NULL)
    {
        // Compares lowercase word from text to words from dictionary inside singly linked list
        if (strcmp(lcase_word, temp->word) != 0)
        {
            temp = temp->next;
        }
        else
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Uses the ASCII decimal of the word first letter
    int word_decimal = *word;

    // Case insensitive
    int index = word_decimal % 97;

    return index;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Opens dictionary file
    FILE *file = fopen(dictionary, "r");

    // Returns failure
    if (file == NULL)
    {
        fclose(file);
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Initializes hashtable buckets to null
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Creates buffer for words from the dictionary
    char word[LENGTH + 1];

    // Reads words from the dictionary
    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        // Returns failure and frees memory
        if (n == NULL)
        {
            free(n);
            return false;
        }

        // Copies word to node
        strcpy(n->word, word);
        n->next = NULL;

        // Hash index
        int index = hash(word);

        // If table[index] is empty, start singly linked list on the index bucket
        if (table[index] == NULL)
        {
            table[index] = n;
            word_counter++;
        }

        // Else add node to the beginning of the singly linked list
        else
        {
            n->next = table[index];
            table[index] = n;
            word_counter++;
        }
    }

    // Closes dictionary file
    fclose(file);

    // Returns sucess
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return *word_counter_ptr;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // Frees hashtable
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        while (list != NULL)
        {
            node *tmp = list->next;
            free(list);
            list = tmp;
        }
    }
    // Returns sucess
    return true;
}
