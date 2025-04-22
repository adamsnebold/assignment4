/*
 * This file contains the definition of an interface for a hash table data structure.
 */

#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

/*
 * Structure used to represent a hash table.
 */
struct hash_table;

/*
 * compue the hash code for the key, version 1
 */
int hash_function1(struct hash_table* hash_table, char* key);

/*
 * compue the hash code for the key, version 2. This is to be 
 * updated in the assignment, within the hash_table.c source code
 */
int hash_function2(struct hash_table* hash_table, char* key);


/*
 * Creates a new, empty hash_table and returns a pointer to it.
 */
struct hash_table* hash_table_create(int array_size);

/*
 * Free all of the memory associated with a hash_table.
 *
 * Params:
 *   hash_table - the hash_table to be destroyed.  May not be NULL.
 */
void hash_table_free(struct hash_table* hash_table);

/*
 * Reset all of the buckets associated with a hash_table.
 *
 * Params:
 *   hash_table - the hash_table to be reset.
 */


void hash_table_reset(struct hash_table* hash_table);

/*
 * add a new value onto a hash_table.
 *
 * Params:
 *   hash_table - the hash_table onto which to add a value.  May not be NULL.
 *   value - the new value to be added onto the hash_table
 */
void hash_table_add(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*),char* key, int value);

/*
 * Removes the front element from a hash_table and returns its value.
 *
 * Params:
 *   hash_table - the hash_table from which to remove a value.  May not be NULL or empty.
 *
 * Return:
 *   returns 1 if the key was found and element removed, 0 otherwise
 */

int hash_table_remove(struct hash_table* hash_table, int (*hf)(struct hash_table*, char*), char* key);

/*
 * Counts the total number of collisions that occured in a full hash table 
 *
 * Params:
 *   hash_table - the hash_table to display
 *
 */

int hash_table_collisions(struct hash_table* hash_table);


/*
 * Prints the contents of a hash table 
 *
 * Params:
 *   hash_table - the hash_table to display
 *
 */

void display(struct hash_table* hash_table);




#endif
