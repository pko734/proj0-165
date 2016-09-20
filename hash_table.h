#ifndef HASH_TABLE_GUARD
#define HASH_TABLE_GUARD

#include <stdlib.h>

typedef int keyType;
typedef int valType;

typedef struct node {
  keyType key;
  valType val;
  struct node *next;
} node;

typedef struct linkedlist {
  size_t size;
  node *head;
  node *tail; 
} linkedlist;

typedef struct hashtable {
  // define the components of the hash table here (e.g. the array, bookkeeping for number of elements, etc)
  size_t size;
  linkedlist **table;
} hashtable;

void init(hashtable** ht);
void cleanup(hashtable** ht);
void put(hashtable* ht, keyType key, valType value);
int get(hashtable* ht, keyType key, valType *values, int num_values);
void erase(hashtable* ht, keyType key);

unsigned int hash(hashtable *ht, keyType key);
void hash_print( hashtable *ht );

void list_init(linkedlist **list);
void list_insert(linkedlist **list, node *node);
int list_get(linkedlist *list, keyType key, valType *values, int num_values);
void list_delete(linkedlist **list, keyType key);
void list_print(linkedlist *list);

void node_init(node **node, keyType key, valType val);
#endif
