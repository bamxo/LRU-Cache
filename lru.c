#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru.h"
#include "set.h"

// Node structure for the LRU
typedef struct LRUNode
{
    char *key;
    void *item;
    struct LRUNode *next;
    struct LRUNode *prev;
} LRUNode;

// LRU structure
struct lru
{
    int capacity;
    int size;
    LRUNode *head;
    LRUNode *tail;
    set_t *set;
};

// Helper function to create a new LRUNode
LRUNode *createLRUNode(const char *key, void *item)
{
    LRUNode *node = (LRUNode *)malloc(sizeof(LRUNode));
    if (node != NULL)
    {
        node->key = strdup(key);
        node->item = item;
        node->next = NULL;
        node->prev = NULL;
    }
    return node;
}

void deleteItem(void *item)
{
    // Your item deletion logic here
    free(item);
}

// Helper function to remove the LRU node
void removeLRUNode(lru_t *lru)
{
    if (lru->head != NULL)
    {
        LRUNode *temp = lru->head;
        lru->head = lru->head->next;
        if (lru->head != NULL)
        {
            lru->head->prev = NULL;
        }
        else 
        {
            lru->tail = NULL;
        }

        // Remove the key from the set
        set_delete(lru->set, NULL);  // Only delete the specific LRUNode, not the entire set

        free(temp->key);
        free(temp);
        lru->size--;
    }
}

lru_t *lru_new(const int capacity) {
    if (capacity <= 0)
    {
        return NULL;  // Invalid capacity
    }

    lru_t *lru = (lru_t *)malloc(sizeof(lru_t));
    if (lru != NULL)
    {
        lru->capacity = capacity;
        lru->size = 0;
        lru->head = NULL;
        lru->tail = NULL;
        lru->set = set_new();  // Initialize the set
    }

    return lru;
}

bool lru_insert(lru_t *lru, const char *key, void *item)
{
    if (lru == NULL || key == NULL || item == NULL)
    {
        return false;  // Invalid parameters
    }

    // Check if the key already exists
    if (set_find(lru->set, key) != NULL)
    {
        return false;
    }

    // Check if the LRU is full
    if (lru->size == lru->capacity)
    {
        removeLRUNode(lru);
    }

    // Create a new LRU node
    LRUNode *node = createLRUNode(key, item);
    if (node == NULL)
    {
        return false;  // Memory allocation failure
    }

    // Add the node to the LRU
    if (lru->head == NULL)
    {
        lru->head = node;
        lru->tail = node;
    }
    else
    {
        node->next = lru->head;
        lru->head->prev = node;
        lru->head = node;
    }

    // Add the key to the set
    set_insert(lru->set, key, item);

    lru->size++;
    return true;
}

void *lru_find(lru_t *lru, const char *key)
{
    if (lru == NULL || key == NULL)
    {
        return NULL;  // Invalid parameters
    }

    // Check if the key exists
    void *item = set_find(lru->set, key);
    if (item == NULL)
    {
        return NULL;  // Key not found
    }

    // Move the accessed node to the front
    LRUNode *node = lru->head;
    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            // Move the accessed node to the front
            if (node != lru->head)
            {
                if (node == lru->tail)
                {
                    lru->tail = node->prev;
                }
                else
                {
                    node->next->prev = node->prev;
                }
                node->prev->next = node->next;
                node->next = lru->head;
                node->prev = NULL;
                lru->head->prev = node;
                lru->head = node;
            }
            break;
        }
        node = node->next;
    }

    return item;
}

// Helper function to print a single item
void printItem(FILE *fp, const char *key, void *item)
{
    fprintf(fp, "%s : %s\n", key, (char *)item);
}

void lru_print(lru_t *lru, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
{
    if (fp == NULL || lru == NULL || itemprint == NULL)
    {
        return;  // Invalid parameters
    }

    // Print the contents of the LRU
    LRUNode *node = lru->head;
    while (node != NULL)
    {
        itemprint(fp, node->key, node->item);
        node = node->next;
    }
}

void lru_iterate(lru_t *lru, void *arg, void (*itemfunc)(void *arg, const char *key, void *item))
{
    if (lru == NULL || itemfunc == NULL)
    {
        return;  // Invalid parameters
    }

    // Iterate over all items in the LRU
    LRUNode *node = lru->head;
    while (node != NULL)
    {
        itemfunc(arg, node->key, node->item);
        node = node->next;
    }
}

void lru_delete(lru_t *lru, void (*itemdelete)(void *item))
{
    if (lru == NULL)
    {
        return;
    }

    LRUNode *node = lru->head;
    LRUNode *next;

    while (node != NULL)
    {
        next = node->next;
        if (itemdelete != NULL)
        {
            itemdelete(node->item);
        }
        free(node->key);
        free(node);
        node = next;
    }

    // Delete the set
    set_delete(lru->set, NULL);

    // Delete the LRU
    free(lru);
}



