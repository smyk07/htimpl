/*
 * includes all the basic tests for the current hash table implementation.
 */

#include "ht.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  ht *table = ht_new();

  ht_insert(table, "name", "mei mei");
  ht_insert(table, "age", "67");
  ht_insert(table, "city", "dubai");

  char *name = ht_search(table, "name");
  printf("Name: %s\n", name);
  assert(strcmp(name, "mei mei") == 0);

  char *age = ht_search(table, "age");
  printf("Age: %s\n", age);
  assert(strcmp(age, "67") == 0);

  char *city = ht_search(table, "city");
  printf("City: %s\n", city);
  assert(strcmp(city, "dubai") == 0);

  char *missing = ht_search(table, "country");
  assert(missing == NULL);

  ht_insert(table, "name", "tole tole");
  char *updated_name = ht_search(table, "name");
  printf("Updated Name: %s\n", updated_name);
  assert(strcmp(updated_name, "tole tole") == 0);

  ht_delete(table, "age");
  char *deleted_age = ht_search(table, "age");
  assert(deleted_age == NULL);

  ht_insert(table, "email", "vro@github.com");
  ht_insert(table, "phone", "123-456-7890");
  ht_insert(table, "address", "123 labubu st");

  char *email = ht_search(table, "email");
  printf("Email: %s\n", email);
  assert(strcmp(email, "vro@github.com") == 0);

  ht_delete(table, "city");
  ht_delete(table, "email");
  ht_delete(table, "phone");

  char *remaining_name = ht_search(table, "name");
  char *remaining_address = ht_search(table, "address");
  assert(strcmp(remaining_name, "tole tole") == 0);
  assert(strcmp(remaining_address, "123 labubu st") == 0);

  ht_delete(table, "nonexistent");

  printf("All tests passed!\n");

  ht_del_ht(table);

  return 0;
}
