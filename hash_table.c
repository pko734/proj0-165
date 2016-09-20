#include <stdio.h>
#include "hash_table.h"

#define HT_INITIAL_SIZE 17

// initialize the components of the hashtable
void init(hashtable** ht) {
  init_custom(ht, HT_INITIAL_SIZE);
}

// a custom init method that lets you specify the number of
// hashtable buckets.
void init_custom(hashtable** ht, unsigned int buckets) {
  unsigned int i;
  *ht = malloc(sizeof(hashtable));
  (*ht)->table = malloc(sizeof(linkedlist *) * buckets );
  for (i=0; i < buckets; i++ ) {
    (*ht)->table[i] = NULL;
  }
  (*ht)->pairs = 0;
  (*ht)->size = buckets;
}

// frees the space allocated for the hash table in init.
void cleanup(hashtable** ht) {
  free( (*ht)->table );
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
  ht->pairs++;
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
  return (list) ? list_get( list, key, values, num_values ) : 0;
}

// erase a key-value pair from the hash table
void erase(hashtable* ht, keyType key) {
  unsigned int hashkey = hash(ht, key);
  linkedlist *list = ht->table[hashkey];
  if (list) {
    ht->pairs -= list_delete( &list, key );
    if( list->size == 0 ) {
      free( list );
      ht->table[hashkey] = NULL;
    }
  }
}

// hash function
unsigned int hash( hashtable *ht, keyType key) {
  return key % ht->size;
}

// optimize the hashtable
void optimize(hashtable** ht) {
  (void)ht;
}

// print contents of the hash table for debugging
void hash_print(hashtable *ht) {
  printf("*********************\n");
  unsigned int i;
  for(i=0; i < ht->size; i++) {
    printf("bucket %d: ", i);
    if (ht->table[i] == NULL) {
      printf("EMPTY\n");
    } else {
      list_print( ht->table[i] );
    }
  }
}

// init an empty linked list.
void list_init(linkedlist **list) {
  *list = malloc(sizeof(struct linkedlist));
  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->size = 0;
}

// insert node into list
void list_insert(linkedlist **list, node *node) {
  if((*list)->size == 0) {
    (*list)->head = node;
  } else {
    ((*list)->tail)->next = node;
  }
  (*list)->tail = node;
  (*list)->size++;
}

// get values from key in linked list, following same
// requirements as "get" above.
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

// delete any nodes from the list containing key.
// return the number of nodes deleted.
int list_delete(linkedlist **list, keyType key) {
  node *prev_node, *this_node, *tmp_node;
  this_node = (*list)->head;
  prev_node = NULL;
  tmp_node  = NULL;
  unsigned int num_deleted = 0;
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
      num_deleted++;
    } else {
      prev_node = this_node;
      this_node = this_node->next;
    }
  }
  return num_deleted;
}

// print a linked list for debugging
void list_print(linkedlist *list) {
  node *node = list->head;
  printf( "(%d) ", (int)list->size );
  while( node ) {
    printf( "[%d,%d] -> ", node->key, node->val );
    node = node->next;
  }
  printf( "NULL\n" );
}

// initialize an empty node.
void node_init(node **node, keyType key, valType val) {
  *node = malloc(sizeof(struct node));
  (*node)->key = key;
  (*node)->val = val;
  (*node)->next = NULL;
}
