/*
 * ht - contains the hash table struct and its interface.
 *
 * Copyright 2025 Samyak Bambole <bambole@duck.com>

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>

/*
 * @struct ht_item: represents an individual item inside a hash table.
 */
typedef struct ht_item {
  char *key;
  void *value;
} ht_item;

/*
 * @struct ht: represents the hash table.
 */
typedef struct ht {
  size_t base_capacity;
  size_t capacity;
  size_t count;

  ht_item **items;
  size_t value_size;
} ht;

/*
 * @brief: initialize a new hash table.
 *
 * @param value_size: size of the value (void *) of each item.
 *
 * @return: pointer to the newly initialized hash table.
 */
ht *ht_new(const size_t value_size);

/*
 * @brief: delete an existing hash table.
 *
 * @param table: pointer to an initialized ht (hash table) struct.
 */
void ht_del_ht(ht *table);

/*
 * @brief: insert a key value pair into the hash table.
 *
 * @param table: pointer to an initialized ht (hash table) struct.
 * @param key: key string literal.
 * @param value: pointer to the value which is to be stored (will be copied).
 */
void ht_insert(ht *table, const char *key, const void *value);

/*
 * @brief: search for a key inside the hash table and retrieve the stored value.
 *
 * @param table: pointer to an initialized ht (hash table) struct.
 * @param key: key string literal
 */
char *ht_search(ht *table, const char *key);

/*
 * @brief: delete a key-value pair from the hash table.
 *
 * @param table: pointer to an initialized ht (hash table) struct.
 * @param key: key string literal.
 */
void ht_delete(ht *table, const char *key);
