/*
 * contains hash table function implementations.
 *
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

#include "ht.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief: check if x is prime.
 */
static int is_prime(const int x) {
  if (x < 2) {
    return -1;
  }
  if (x < 4) {
    return 1;
  }
  if ((x % 2) == 0) {
    return 0;
  }
  for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
    if ((x % i) == 0) {
      return 0;
    }
  }
  return 1;
}

/*
 * @brief: find the next prime number which is greater than x.
 */
static int next_prime(int x) {
  while (is_prime(x) != 1) {
    x++;
  }
  return x;
}

/*
 * @brief: a simple hash function.
 *
 * @param s: string to hash
 * @param prime: a prime number
 * @param m: size of the hash table
 */
static inline int ht_hash(const char *s, const int prime, const int m) {
  long hash = 0;
  const int len_s = strlen(s);

  for (int i = 0; i < len_s; i++) {
    hash += (long)pow(prime, len_s - (i + 1)) * s[1];
    hash = hash % m;
  }

  return (int)hash;
}

/*
 * @brief: implements double-hashing to avoid collissions.
 *
 * @param s: string to hash
 * @param num_buckets: size of the hash table
 * @param attempts: number of attempts it took to hash the current string
 */
static int ht_get_hash(const char *s, const int num_buckets,
                       const int attempt) {
#define HT_PRIME_1 0x21914047
#define HT_PRIME_2 0x1b873593
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
#undef HT_PRIME_1
#undef HT_PRIME_2
}

/*
 * @brief: allocate and initialize a new ht_item and return its memory address.
 */
static ht_item *ht_new_item(const char *k, const char *v) {
  ht_item *i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

/*
 * @brief: free / delete an existing ht_item.
 */
static void ht_del_item(ht_item *i) {
  free(i->key);
  free(i->value);
  free(i);
}

static ht_item HT_DELETED_ITEM = {NULL, NULL};

/*
 * @brief : create a new sized hash table
 */
static ht *ht_new_sized(const int base_size) {
  ht *table = malloc(sizeof(ht));
  table->base_size = base_size;

  table->size = next_prime(table->base_size);

  table->count = 0;
  table->items = calloc(table->size, sizeof(ht_item *));

  return table;
}

ht *ht_new() { return ht_new_sized(53); }

/*
 * @brief: resize an existing hash table to avoid high collission rates and keep
 * storing more key-value pairs.
 */
static void ht_resize(ht *table, const int base_size) {
  if (base_size < 53) {
    return;
  }

  ht *new_ht = ht_new_sized(base_size);
  for (int i = 0; i < table->size; i++) {
    ht_item *item = table->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_insert(new_ht, item->key, item->value);
    }
  }

  table->base_size = new_ht->base_size;
  table->count = new_ht->count;

  const int tmp_size = table->size;
  table->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item **tmp_items = table->items;
  table->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_ht(new_ht);
}

/*
 * @brief: wrapper function to make the hash table bigger.
 */
static void ht_resize_up(ht *ht) {
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

/*
 * @brief: wrapper function to make the hash table smaller.
 */
static void ht_resize_down(ht *ht) {
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}

void ht_del_ht(ht *table) {
  if (table == NULL)
    return;

  if (table->items != NULL) {
    for (size_t i = 0; i < table->size; i++) {
      ht_item *item = table->items[i];
      if (item != NULL && item != &HT_DELETED_ITEM) { // Skip sentinel
        ht_del_item(item);
      }
    }
    free(table->items);
  }
  free(table);
}

void ht_insert(ht *ht, const char *key, const char *value) {
  const int load = ht->count * 100 / ht->size;

  if (load > 70) {
    ht_resize_up(ht);
  }

  ht_item *item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item *current = ht->items[index];

  int i = 1;
  while (current != NULL) {
    if (current != &HT_DELETED_ITEM) {
      if (strcmp(current->key, key) == 0) {
        ht_del_item(current);
        ht->items[index] = item;
        return;
      }
    }
    index = ht_get_hash(item->key, ht->size, i);
    current = ht->items[index];
    i++;
  }

  ht->items[index] = item;
  ht->count++;
}

char *ht_search(ht *ht, const char *key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];

  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        return item->value;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }

  return NULL;
}

void ht_delete(ht *ht, const char *key) {
  const int load = ht->count * 100 / ht->size;

  if (load < 10) {
    ht_resize_down(ht);
  }

  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];

  int i = 0;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }

  ht->count++;
}
