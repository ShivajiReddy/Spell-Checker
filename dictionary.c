// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

typedef struct node
{
    bool is_word;
    struct node* children[27];
} node;

//Functions Declaration
int get_index(char c);
node* get_node();
bool unload_helper(node* node_ptr);


int word_count = 0; 
node* first;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    node* node_ptr = first;
    for (int i = 0, len = strlen(word); i <= len; ++i)
    {   if (word[i] == '\0')
        {
            if (node_ptr->is_word)
            {
                return true;
            }
            else return false;
        }

        int index = get_index(word[i]);
        if(node_ptr->children[index] == NULL)
            return false;
        
        node_ptr = node_ptr->children[index];
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE* dict_ptr = fopen(dictionary, "r");
    if (dict_ptr == NULL)
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }

    first = get_node();
    int index;

    //Extracting the dictionary words
    node *node_ptr = first;
    for(char c = fgetc(dict_ptr); c != EOF; c = fgetc(dict_ptr))
    {
        if (c != '\n')
        {
            index = get_index(c);
            if(!node_ptr->children[index])
                node_ptr->children[index] = get_node();
            node_ptr = node_ptr->children[index];
        }
        else
        {
            //Mark the end of the word
            node_ptr->is_word = true;
            word_count++;
            //Go back to homebase
            node_ptr = first;
        }

    }
    return true;
}

int get_index(char c)
{
    if(c == '\'')
        return 26;
    else if (c >= 'a' && c <= 'z')
    {
        return c-'a';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c-'A';
    }

    return -1;
}

node* get_node()
{
    node* node_ptr = malloc(sizeof(node));
    node_ptr->is_word = false;
    for (int i = 0; i < 27; ++i)
    {
        node_ptr->children[i] = NULL;
    }

    return node_ptr;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    if(unload_helper(first)) 
        return true;
    
    return false;
}

bool unload_helper(node* node_ptr)
{
    for (int i = 0; i < 27; ++i)
    {
        if(node_ptr->children[i] != NULL)
        unload_helper(node_ptr->children[i]);
    }
    free(node_ptr);

    return true;
}