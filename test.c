/*
 * This file contains executable code for testing your work in this assignment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "node.h"
#include "hash_table.h"
 

int NUM_TESTING_PRODUCTS = 11;

/*
 * These are the names of the products that'll be used for testing.
 */
char* TEST_NAMES[] = {
  "apples",
  "apricots",
  "soup",
  "milk",
  "tofu",
  "poptarts",
  "lightbulbs",
  "soda",
  "chips",
  "cheese",
  "cheetos"
};


/*
 * These are the product inventorys for the products in the array above that will be
 * used for testing.
 */
int TEST_INVENTORIES[] = {
  7,
  6,
  6,
  3,
  1,
  2,
  0,
  5,
  24,
  12,
  7
};

int main(int argc, char** argv) {
  int array_size = 8;

  struct hash_table* hash_table = hash_table_create(array_size);

  /*
   *  FIRST, how does it perform using hash_function1() ? 
   */

  for(int i=0; i<NUM_TESTING_PRODUCTS; i++) {
    hash_table_add(hash_table, hash_function1, TEST_NAMES[i], TEST_INVENTORIES[i]);
  }

  int num_col1 = hash_table_collisions(hash_table);
  printf("Found %d collisions for hash_function1()\n",num_col1);

  display(hash_table);

  hash_table_reset(hash_table);
  

  /*
   *  SECOND, how does it perform using hash_function2() ? 
   * 
   *  currently, hash_function2() is the same a 1, but your assignment is to 
   *  modify it to create a better hash function. 
   */

  for(int i=0; i<NUM_TESTING_PRODUCTS; i++) {
    hash_table_add(hash_table, hash_function2, TEST_NAMES[i], TEST_INVENTORIES[i]);
  }

  int num_col2 = hash_table_collisions(hash_table);
  printf("Found %d collisions for hash_function2()\n",num_col2);

  display(hash_table);

  printf("As an example of removal, here is the\n");
  printf("final hash table with %s removed\n\n",TEST_NAMES[9]);
  hash_table_remove(hash_table, hash_function2, TEST_NAMES[9]);
  display(hash_table);

  hash_table_free(hash_table);
}
