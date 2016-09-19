#include <stdio.h>
#include "hash_table.h"

#define HT_INITIAL_SIZE 10

// initialize the components of the hashtable
void init(hashtable** ht) {
  unsigned int i;
  *ht = malloc(sizeof(hashtable));
  (*ht)->table = malloc(sizeof(linkedlist *) * HT_INITIAL_SIZE );
  for (i=0; i < HT_INITIAL_SIZE; i++ ) {
    (*ht)->table[i] = NULL;
  }
  (*ht)->size = HT_INITIAL_SIZE;
}

// insert a key-value pair into the hash table
void put(hashtable* ht, keyType key, valType value) {
  unsigned int hashkey = hash(ht, key);
  linkedlist *list = ht->table[hashkey];
  node *new_node = NULL;

  node_init( &new_node, key, value );

  if (list == NULL) {
    list_init(&list);
  }
  
  list_insert(&list, new_node);  
  ht->table[hashkey] = list;
}

// get entries with a matching key and stores the corresponding values in the
// values array. The size of the values array is given by the parameter
// num_values. If there are more matching entries than num_values, they are not
// stored in the values array to avoid a buffer overflow. The function returns
// the number of matching entries. If the return value is greater than
// num_values, the caller can invoke this function again (with a larger buffer)
// to get values that it missed during the first call. 
int get(hashtable* ht, keyType key, valType *values, int num_values) {
  unsigned int hashkey = hash(ht, key);
  linkedlist *list = ht->table[hashkey];
  return list ? list_get( list, key, values, num_values ) : 0;
}

// erase a key-value pair from the hash talbe
void erase(hashtable* ht, keyType key) {
  unsigned int hashkey = hash(ht, key);
  linkedlist *list = ht->table[hashkey];
  if (list) {
    list_delete( &list, key );
    if( list->size == 0 ) {
      free( list );
      ht->table[hashkey] = NULL;
    }
  }
}

unsigned int hash( hashtable *ht, keyType key) {
  return key % ht->size;
}

void hash_print(hashtable *ht) {
  printf("*********************\n");
  unsigned int i;
  for(i=0; i < ht->size; i++) {
    printf("hashkey: %d\n", i);
    if (ht->table[i] == NULL) {
      printf("EMPTY\n");
    } else {
      list_print( ht->table[i] );
    }
    printf("\n");
  }
}

void list_init(linkedlist **list) {
  *list = malloc(sizeof(linkedlist));
  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->size = 0;
}

void list_insert(linkedlist **list, node *node) {
  if((*list)->size == 0) {
    (*list)->head = node;
  } else {
    ((*list)->tail)->next = node;
  }
  (*list)->tail = node;
  (*list)->size++;
}

int list_get(linkedlist *list, keyType key, valType *values, int num_values) {
  node *this_node;
  unsigned int found = 0;

  this_node = list->head;
  
  while(this_node && (found < (unsigned int)num_values)) {
    if (this_node->key == key) {
      values[found++] = this_node->val;
    }
    this_node = this_node->next;
  }
  return found;
}

void list_delete(linkedlist **list, keyType key) {
  node *prev_node, *this_node, *tmp_node;
  this_node = (*list)->head;
  prev_node = NULL;
  tmp_node  = NULL;
  while(this_node) {
    if (this_node->key == key) {
      if (prev_node) {
        prev_node->next = this_node->next;
      }
      if (this_node == (*list)->head) {
	(*list)->head = this_node->next;
      }
      if (this_node == (*list)->tail) {
        (*list)->tail = prev_node;
      }
      tmp_node = this_node->next;      
      free( this_node );
      this_node = tmp_node;
      (*list)->size--;
    } else {
      prev_node = this_node;
      this_node = this_node->next;
    }
  }  
}

void list_print(linkedlist *list) {
  node *node = list->head;
  printf( "list size: %d\n", (int)list->size );
  while( node ) {
    printf( "key: %d, val: %d, next: %p\n", node->key, node->val, node->next );
    node = node->next;
  }
}

void node_init(node **node, keyType key, valType val) {
  *node = malloc(sizeof(node));
  (*node)->key = key;
  (*node)->val = val;
  (*node)->next = NULL;
}
