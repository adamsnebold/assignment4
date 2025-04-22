/*
 * This file contains the definitions of structures and functions implementing
 * a simple hash_table using a linked list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node.h"
#include "hash_table.h"

/*
 * This is the definition of the hash_table structure.  Using a linked list to
 * implement a hash_table requires that we keep track of both the first and the
 * last of the hash_table.
 */
struct hash_table {
  struct node** array;
  int size;
  int total;
};


/*
 * Returns: a hash code of an input string "key"
 * 
 * input: the hash_table (to get the size) and a key
 */

int hash_function1(struct hash_table* hash_table, char* key) {
  return ( (int) key[0] ) % hash_table->size;
}


/*
 * Returns: a hash code of an input string "key"
 * 
 * input: the hash_table (to get the size) and a key
 */

int hash_function2(struct hash_table* hash_table, char* key) {
  /*
   * Currently this is the same as hash_function2, but your assignment is 
   * to modify it to create an improved hash function: 
   */

  return ( (int) key[0] ) % hash_table->size;
}

struct hash_table* hash_table_create(int array_size) {
  struct hash_table* hash_table = malloc(sizeof(struct hash_table));
  assert(hash_table);
  hash_table->total = 0;
  hash_table->size = array_size;

  // allocate memory for the array, assign lists to NULL 
  hash_table->array = malloc(array_size * sizeof(struct node*));

  // assign each term to NULL
  for(int i=0; i < hash_table->size; i++) {
    hash_table->array[i] = NULL;
  }

  // return the hash_table
  return hash_table;
}

void hash_table_free(struct hash_table* hash_table) {
  assert(hash_table);
  /*
   * loop through the array, and delete each node
   */
  for(int i=0; i < hash_table->size; i++) {  
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      hash_table->array[i] = current->next;
      assert(current->key);
      free(current->key);
      assert(current);
      free(current);
      current  = hash_table->array[i];
    }
  }
  free(hash_table);
}

void hash_table_reset(struct hash_table* hash_table) {
  assert(hash_table);
  /*
   * loop through the array, and delete each node
   */
  for(int i=0; i < hash_table->size; i++) {  
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      hash_table->array[i] = current->next;
      assert(current->key);
      free(current->key);
      assert(current);
      free(current);
      current = hash_table->array[i];
      // reduce the total by 1 for each removed node
      hash_table->total--;
    }
  }
  // at the point, the hash_table should be an array with NULL pointers only
}

void hash_table_add(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key, int value) {
  assert(hash_table);
  /*
   * Fill out the new node at put it at the beginning of the linked list 
   * for the hash index, defined using the key/name of the product 
   */
  struct node* new_node = malloc(sizeof(struct node));
  assert(new_node);

  new_node->key = (char*) malloc((strlen(key)+1)*sizeof(char));
  strcpy(new_node->key,key);
  new_node->value = value;
  new_node->next = NULL;

  //int hash_index = hash_function(hash_table, key);
  int hash_index = (*hf)(hash_table, key);

  // add the node to the beginning of the list 
  new_node->next = hash_table->array[hash_index];
  hash_table->array[hash_index] = new_node;

  // update the total to reflect new addition
  hash_table->total++;
}

int hash_table_remove(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key) {
  assert(hash_table);
  assert(hash_table->array);

  //int hash_index = hash_function(hash_table, key);
  int hash_index = (*hf)(hash_table, key);
  
  // in this case the key is associated with the first node 
  struct node* temp = hash_table->array[hash_index];
  if( temp != NULL ) {
    if( strcmp(temp->key,key) == 0) {    
      printf("removing %s from hash table, should match %s\n", temp->key, key);
      hash_table->array[hash_index] = temp->next;
      assert(temp->key);
      //free(temp->key);
      assert(temp);
      free(temp);
      hash_table->total--;
      return 1;
    }
  }

  // in this case the product is not the first one 
  // define another node "prev" for the previous node. 
  struct node* prev;

  while (temp != NULL && strcmp(temp->key, key) != 0) {
    prev = temp;
    temp = temp->next;    
  }

  // if temp is NULL, then the key was not found. 
  if(temp == NULL) {
    printf("The key %s not found in hash table. ", key);
    return 0;
  }

  // if temp was not NULL, then remove the found node and free it 
  prev->next = temp->next;
  printf("trying to free: %s\n",temp->key);
  assert(temp->key);
  free(temp->key);
  assert(temp);
  free(temp);

  return 1;
}

int hash_table_collisions(struct hash_table* hash_table) {
  /*
   * This function should count the number of collisions in the hash table
   *
   * this would essentially be the total number of elements above 1 for each 
   * "bucket" or hash array index. If there is only one element associated 
   * with a bucket, it would not count as a collision. Two elements would count
   * as one collision, three elements would count as 2 collisions and so on. 
   */
  int num_col = 0;
  
  // code goes here: 

  return num_col;
}

void display(struct hash_table* hash_table) {
  
  printf("Hash table, size=%d, total=%d\n",hash_table->size, hash_table->total);
  int i = 0;
  for (i = 0; i < hash_table->size; i++) {
    struct node *temp = hash_table->array[i];
    if (temp == NULL) {
      printf("array[%d]-|\n", i);
    } else {
      printf("array[%d]", i);
      while (temp != NULL) {
	printf("->(key=%s,value=%d)", temp->key, temp->value);
	temp = temp->next;
      }
      printf("-|\n");
    }
  }
  printf("\n");
}
