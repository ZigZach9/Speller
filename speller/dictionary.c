// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27
int count = 0;
// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        //Pointer starts at root for each new word
        node *ptr = root;
        //int c is for children[c]
        int c = 0;
        for (int j = 0; word[j] != 0; j++ )
        {
            //Convert the letter in word to the corect position number for the children array
            if (word[j] == 39)
            {
                c = 26;
            } else {
                c = word[j] - 97;
            }
            if (ptr->children[c] == NULL)
            {
                //Malloc a new node
                node *n = malloc(sizeof(node));
                if (n == NULL)
                {
                    return false;
                }
                n->is_word = false;
                for (int k = 0; k < N; k++)
                {
                    n->children[k] = NULL;
                }
                //Assign this new node to child[c] of previous node
                ptr->children[c] = n;
            }
            //Pointer moves to new node
            ptr = ptr->children[c];
            //If you are on the last letter of the word, then set is.word to true
            if (j == strlen(word) - 1)
            {
                ptr->is_word = true;
                count ++;
            }
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (count != 0)
    {
        return count;
    }
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Pointer starts at root
    node *ptr = root;
    //c is for children[c]
    int c = 0;
    for (int i = 0; word[i] != 0; i++)
    {
        //Convert the letter in word to the corect position number for the children array
        if (word[i] == 39)
        {
            c = 26;
        } else if (word[i] >= 'A' && word[i] <= 'Z') {
            c = word[i] - 65;
        }
        else {
            c = word[i] - 97;
        }
        if (ptr->children[c] == NULL)
        {
            return false;
        }
        ptr= ptr->children[c];
        //If you are on the last letter of the word, then check if is.word is false
        if (i == strlen(word) - 1)
        {
            if (ptr->is_word == false)
            {
                return false;
            }
        }
    }
    return true;
}

//checks if last node, returns 0 when freed
void lastNode(node *stuff)
{
    if (stuff == NULL)
    {
        return;
    }

    for (int i = 0; i < N; i++)
    {
        lastNode(stuff->children[i]);
    }
    free(stuff);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    lastNode(root);
    return true;
}
