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
 * This is the definition of the hash_table structure. Using a linked list to
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
  return ((int) key[0]) % hash_table->size;
}

/*
 * Returns: a hash code of an input string "key"
 * 
 * input: the hash_table (to get the size) and a key
 *
 * UPDATED: Improved hash function using the djb2 algorithm.
 */
int hash_function2(struct hash_table* hash_table, char* key) {
  unsigned long hash = 5381;
  int c;
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;  // hash * 33 + c
  }
  return (int)(hash % hash_table->size);
}

struct hash_table* hash_table_create(int array_size) {
  struct hash_table* hash_table = malloc(sizeof(struct hash_table));
  assert(hash_table);
  hash_table->total = 0;
  hash_table->size = array_size;

  // allocate memory for the array, assign lists to NULL 
  hash_table->array = malloc(array_size * sizeof(struct node*));

  // assign each term to NULL
  for(int i = 0; i < hash_table->size; i++) {
    hash_table->array[i] = NULL;
  }

  return hash_table;
}

void hash_table_free(struct hash_table* hash_table) {
  assert(hash_table);

  // loop through the array, deleting each node
  for(int i = 0; i < hash_table->size; i++) {  
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      hash_table->array[i] = current->next;
      assert(current->key);
      free(current->key);
      assert(current);
      free(current);
      current = hash_table->array[i];
    }
  }
  free(hash_table);
}

void hash_table_reset(struct hash_table* hash_table) {
  assert(hash_table);

  // loop through the array, deleting each node
  for(int i = 0; i < hash_table->size; i++) {  
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      hash_table->array[i] = current->next;
      assert(current->key);
      free(current->key);
      assert(current);
      free(current);
      current = hash_table->array[i];
      hash_table->total--;  // decrement for each removed node
    }
  }
}

void hash_table_add(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key, int value) {
  assert(hash_table);

  // Allocate new node and prepare its contents.
  struct node* new_node = malloc(sizeof(struct node));
  assert(new_node);

  new_node->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->next = NULL;

  // Determine hash index using the provided hash function.
  int hash_index = (*hf)(hash_table, key);

  // Insert at the beginning of the linked list at the hash index.
  new_node->next = hash_table->array[hash_index];
  hash_table->array[hash_index] = new_node;

  hash_table->total++;
}

int hash_table_remove(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key) {
  assert(hash_table);
  assert(hash_table->array);

  int hash_index = (*hf)(hash_table, key);

  // Check if the key is in the first node of the bucket.
  struct node* temp = hash_table->array[hash_index];
  if (temp != NULL && strcmp(temp->key, key) == 0) {
    printf("removing %s from hash table, should match %s\n", temp->key, key);
    hash_table->array[hash_index] = temp->next;
    assert(temp->key);
    free(temp);
    hash_table->total--;
    return 1;
  }

  // Traverse the linked list in this bucket to find the key.
  struct node* prev;
  while (temp != NULL && strcmp(temp->key, key) != 0) {
    prev = temp;
    temp = temp->next;    
  }

  if (temp == NULL) {
    printf("The key %s not found in hash table. ", key);
    return 0;
  }

  // Remove the node containing the key.
  prev->next = temp->next;
  printf("trying to free: %s\n", temp->key);
  assert(temp->key);
  free(temp->key);
  assert(temp);
  free(temp);

  return 1;
}

/*
 * Counts the total number of collisions in the hash table. For each bucket, if there are
 * more than one node, the excess nodes (n - 1) are counted as collisions.
 */
int hash_table_collisions(struct hash_table* hash_table) {
  int num_col = 0;
  
  for (int i = 0; i < hash_table->size; i++) {
    int count = 0;
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      count++;
      current = current->next;
    }
    if (count > 1)
      num_col += (count - 1);
  }
  
  return num_col;
}

void display(struct hash_table* hash_table) {
  printf("Hash table, size=%d, total=%d\n", hash_table->size, hash_table->total);
  for (int i = 0; i < hash_table->size; i++) {
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
