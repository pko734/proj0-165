#include <stdlib.h>
#include <stdio.h>

#include "hash_table.h"

// This is where you can implement your own tests for the hash table
// implementation. 
int main(void) {

  hashtable *ht = NULL;
  init(&ht, 2);

  int key = 1;
  int value = -1;

  put(ht, key, value);
  put(ht, 2, 2);
  put(ht, 3, 2);
  put(ht, 4, 3);
  put(ht, 1, 2);
  put(ht, 6, 10);
  put(ht, 7, 2);
  put(ht, 1, 100);
  
  //list_print( ht->table[0] );
  hash_print( ht );

  //list_erase( &(ht->table[0]), 1 );
  erase(ht, 1);

  hash_print( ht );
  
  int num_values = 1;

  valType* values = malloc(1 * sizeof(valType));

  int num_results = get(ht, key, values, num_values);
  if (num_results > num_values) {
    values = realloc(values, num_results * sizeof(valType));
    get(ht, 0, values, num_results);
  }

  for (int i = 0; i < num_results; i++) {
    printf("value %d is %d \n", i, values[i]);
  }
  free(values);

  erase(ht, 0);

  free(ht);
  return 0;
}
