/*
 * This file contains the definitions of structures and functions implementing
 * a simple hash_table using a linked list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>    // for math functions

#include "node.h"
#include "hash_table.h"

/*
 * Definition of the hash_table structure.
 * Uses an array of pointers to linked lists (buckets), the size of the table,
 * and a total count of inserted elements.
 */
struct hash_table {
  struct node** array;
  int size;
  int total;
};

/*
 * Returns: a hash code of an input string "key" using a naïve scheme.
 *
 * Note: This function only uses the first character of the key.
 */
int hash_function1(struct hash_table* hash_table, char* key) {
  return ((int) key[0]) % hash_table->size;
}

/*
 * Returns: a hash code of an input string "key" using an improved scheme.
 */
int hash_function2(struct hash_table* hash_table, char* key) {
  unsigned long hash_val = 0;
  int c;
  
  // Convert the entire string to an integer using a multiplier of 31.
  while ((c = *key++)) {
    hash_val = hash_val * 31 + c;
  }
  
  // Multiplicative hashing: multiply by A, take fractional part, then scale.
  double A = 0.6180339887;
  double product = (double) hash_val * A;
  double frac = product - (unsigned long) product;  // fractional part extraction
  int index = (int)(frac * hash_table->size);
  
  return index;
}

/*
 * Creates a new, empty hash_table with the specified array_size.
 */
struct hash_table* hash_table_create(int array_size) {
  struct hash_table* hash_table = malloc(sizeof(struct hash_table));
  assert(hash_table);
  hash_table->total = 0;
  hash_table->size = array_size;
  
  // Allocate the array and initialize all buckets to NULL.
  hash_table->array = malloc(array_size * sizeof(struct node*));
  for (int i = 0; i < hash_table->size; i++) {
    hash_table->array[i] = NULL;
  }
  
  return hash_table;
}

/*
 * Frees all the memory associated with the hash_table.
 */
void hash_table_free(struct hash_table* hash_table) {
  assert(hash_table);
  for (int i = 0; i < hash_table->size; i++) {
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
  free(hash_table->array);
  free(hash_table);
}

/*
 * Resets the hash_table by removing all nodes in every bucket.
 */
void hash_table_reset(struct hash_table* hash_table) {
  assert(hash_table);
  for (int i = 0; i < hash_table->size; i++) {
    struct node* current = hash_table->array[i];
    while (current != NULL) {
      hash_table->array[i] = current->next;
      assert(current->key);
      free(current->key);
      assert(current);
      free(current);
      current = hash_table->array[i];
      // Decrease the total for each removed node.
      hash_table->total--;
    }
  }
}

/*
 * Adds a new (key, value) pair to the hash_table using the provided hash function.
 */
void hash_table_add(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key, int value) {
  assert(hash_table);
  struct node* new_node = malloc(sizeof(struct node));
  assert(new_node);
  
  new_node->key = (char*) malloc((strlen(key) + 1) * sizeof(char));
  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->next = NULL;
  
  int hash_index = (*hf)(hash_table, key);
  
  // Insert new node at the beginning of the list at the computed bucket.
  new_node->next = hash_table->array[hash_index];
  hash_table->array[hash_index] = new_node;
  
  hash_table->total++;
}

/*
 * Removes the node with the matching key from the hash_table.
 *
 * Returns 1 if the removal was successful, 0 if the key was not found.
 */
int hash_table_remove(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key) {
  assert(hash_table);
  assert(hash_table->array);
  
  int hash_index = (*hf)(hash_table, key);
  
  // First, check if the key is at the start of the bucket.
  struct node* temp = hash_table->array[hash_index];
  if (temp != NULL && strcmp(temp->key, key) == 0) {
    printf("removing %s from hash table, should match %s\n", temp->key, key);
    hash_table->array[hash_index] = temp->next;
    assert(temp->key);
    free(temp->key);
    free(temp);
    hash_table->total--;
    return 1;
  }
  
  // Otherwise, search through the list.
  struct node* prev;
  while (temp != NULL && strcmp(temp->key, key) != 0) {
    prev = temp;
    temp = temp->next;
  }
  
  // If key is not found.
  if (temp == NULL) {
    printf("The key %s not found in hash table.\n", key);
    return 0;
  }
  
  // Remove the node with the matching key.
  prev->next = temp->next;
  printf("trying to free: %s\n", temp->key);
  assert(temp->key);
  free(temp->key);
  free(temp);
  
  return 1;
}

/*
 * Counts the total number of collisions in the hash_table.
 *
 * For each bucket:
 *   If the bucket contains n nodes, then (n - 1) collisions occurred.
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
    if (count > 1) {
      num_col += (count - 1);
    }
  }
  
  return num_col;
}

/*
 * Displays the content of the hash_table.
 */
void display(struct hash_table* hash_table) {
  printf("Hash table, size=%d, total=%d\n", hash_table->size, hash_table->total);
  for (int i = 0; i < hash_table->size; i++) {
    struct node* temp = hash_table->array[i];
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
