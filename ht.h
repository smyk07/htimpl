/*
 * ht - contains the hash table struct and its interface.
 */

#include <stddef.h>

/*
 * @struct ht_item: represents an individual item inside a hash table.
 */
typedef struct ht_item {
  char *key;
  char *value;
} ht_item;

/*
 * @struct ht: represents the hash table.
 */
typedef struct ht {
  size_t base_size;
  size_t size;
  size_t count;
  ht_item **items;
} ht;

/*
 * @brief: initialize a new hash table.
 *
 * @return: pointer to the newly initialized hash table.
 */
ht *ht_new();

/*
 * @brief: delete an existing hash table.
 *
 * @param table: pointer to an initialized ht (hash table) struct.
 */
void ht_del_ht(ht *ht);

/*
 * @brief: insert a key value pair into the hash table.
 *
 * @param ht: pointer to an initialized ht (hash table) struct.
 * @param key
 * @param value
 */
void ht_insert(ht *ht, const char *key, const char *value);

/*
 * @brief: search for a key inside the hash table and retrieve the stored value.
 *
 * @param ht: pointer to an initialized ht (hash table) struct.
 * @param key
 */
char *ht_search(ht *ht, const char *key);

/*
 * @brief: delete a key-value pair from the hash table.
 *
 * @param ht: pointer to an initialized ht (hash table) struct.
 * @param key
 */
void ht_delete(ht *ht, const char *key);
