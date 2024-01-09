#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define No_Buckets 17



static bool value_equiv(elem_t key_ignored, elem_t value, void *arg, bool x)
{
    elem_t *other_value_ptr = arg;
    char *other_value = other_value_ptr->string_value;
    return strcmp(value.string_value, other_value) == 0;
}

bool string_eq(elem_t arg1, elem_t arg2) {
  return strcmp(arg1.string_value, arg2.string_value);
}


/*
// Uses the DJB2 algorithm to hash the string
int string_to_int(elem_t str) {
    int hash = 5381;
    unsigned char *ptr = (unsigned char *)str.string_value;

    while (*ptr) {
        hash = ((hash << 5) + hash) + *ptr; // hash * 33 + c
        ptr++;
    }

    return hash;
}

int string_to_int(elem_t str) {
  int counter = 0;
  int value = 0;
  char *st = str.string_value;
  while (st[counter] != '\0') {
    value = value + st[counter];
    counter = counter + 1;
  }
  return value;
}
*/


// If the strings are numbers
int string_to_int(elem_t str) {
  return atoi(str.string_value);
}


int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_create_destroy(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);
  ioopm_hash_table_t *ht_str = ioopm_hash_table_create(string_to_int, NULL);

  // HUGE Test
  
  ioopm_hash_table_t *ht_huge_key_int = ioopm_hash_table_create(NULL, NULL);
  ioopm_hash_table_t *ht_huge_key_string = ioopm_hash_table_create(string_to_int, NULL);

  for (int i = 0; i <= 16; ++i) {
    ioopm_option_t answer = ioopm_hash_table_lookup(ht, int_elem(i));
    char *struct_value = answer.value.string_value;
    CU_ASSERT_PTR_NULL(struct_value);
  }

  // Checks that insert works combined with lookup.

  // Insert with strings as keys
  // Insert in an empty list
  ioopm_hash_table_insert(ht_str, ptr_elem("1"), int_elem(1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_str, ptr_elem("1")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(1)));

  // Insert last in a list 
  ioopm_hash_table_insert(ht_str, ptr_elem("35"), int_elem(35));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_str, ptr_elem("35")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(35)));

  // Insert in between two keys
  ioopm_hash_table_insert(ht_str, ptr_elem("18"), int_elem(18));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_str, ptr_elem("18")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(18)));

  // Change value in the first key
  ioopm_hash_table_insert(ht_str, ptr_elem("1"), int_elem(2));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(2)));

  // Change value in the middle key
  ioopm_hash_table_insert(ht_str, ptr_elem("18"), int_elem(19));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(19)));

  // Change value in the last key
  ioopm_hash_table_insert(ht_str, ptr_elem("35"), int_elem(36));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_str, int_elem(36)));


  // Insert with ints as keys
  // Insert in an empty list
  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Apple"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(6)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Apple")))

  // Insert last in a list
  ioopm_hash_table_insert(ht, int_elem(40), ptr_elem("Banana"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(40)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Banana")))

  // Insert in between two keys
  ioopm_hash_table_insert(ht, int_elem(23), ptr_elem("Mango"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(23)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Mango")));


  // Change the value in the first key
  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Paprika"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Paprika")));

  // Change the value in the middle key
  ioopm_hash_table_insert(ht, int_elem(23), ptr_elem("Cucumber"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Cucumber")));

  // Change the value in the last key
  ioopm_hash_table_insert(ht, int_elem(23), ptr_elem("Pineapple"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Pineapple")));


  // Checks that inserts works for negative integer values for key.
  ioopm_hash_table_insert(ht, int_elem(-1), ptr_elem("Tomato"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Tomato")));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(-1)).success); 
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(-1)));

  // Change the value with negative key
  ioopm_hash_table_insert(ht, int_elem(-1), ptr_elem("Avocado"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Avocado")));

  // Test to insert at 0
  ioopm_hash_table_insert(ht, int_elem(0), ptr_elem("ju8emjihuj766hui"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("ju8emjihuj766hui")));

  // Change value with key 0
  ioopm_hash_table_insert(ht, int_elem(0), ptr_elem("Chicken"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Chicken")));


  // HUGE Test
  
  /* 
  Test HUGE int keys
  */
  for (int i = 0; i < 1257; i++) {
    ioopm_hash_table_insert(ht_huge_key_int, int_elem(i), int_elem(2*i));
  }
  for (int i = 0; i < 1257; i++) {
    // Make sure all the keys are inserted in the hash_table
    ioopm_option_t inserted = ioopm_hash_table_lookup(ht_huge_key_int, int_elem(i));
    bool is_there = inserted.success;
    elem_t the_value = inserted.value;
    CU_ASSERT_TRUE(is_there);
    CU_ASSERT_EQUAL(the_value.int_value, i*2);
    bool key_is_there = ioopm_hash_table_has_key(ht_huge_key_int, int_elem(i));
    CU_ASSERT_TRUE(key_is_there);
  }
  // Test size works for big numbers.
  CU_ASSERT_EQUAL(1257, ioopm_hash_table_size(ht_huge_key_int));
  CU_ASSERT_TRUE(1257 == ioopm_hash_table_size(ht_huge_key_int));

  // Make sure keys list contains all the correct keys.
  ioopm_list_t *list_of_key_ints = ioopm_hash_table_keys(ht_huge_key_int);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_huge_key_int), ioopm_linked_list_size(list_of_key_ints));
  CU_ASSERT_TRUE(1257 == ioopm_linked_list_size(list_of_key_ints));
  for (int i = 0; i < 1257; i++) {
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list_of_key_ints, int_elem(i)));
  }

  /*
  Test HUGE string keys
  */
  
  


  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_destroy(ht_str);
  // For the big ones
  ioopm_hash_table_destroy(ht_huge_key_int);
  ioopm_hash_table_destroy(ht_huge_key_string);
  ioopm_linked_list_destroy(list_of_key_ints);

}

void test_hash_table_size(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  // Checks that size of empty hashtable is 0.
   int empty_hash_table = ioopm_hash_table_size(ht);
   CU_ASSERT_EQUAL(0, empty_hash_table);

  // Checks that size of hashtable is one.
  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Hej"));
  int one_entry = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(1, one_entry);
  // ioopm_hash_table_remove(ht, int_elem(-6));
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, int_elem(6)).success);
  empty_hash_table = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(0, empty_hash_table);

  // Checks that size of hashtable is the_size
  int the_size = 5;
  for (int i = 1; i <= 5; i++) {
    ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("Hello"));
  }
  int five_entries = ioopm_hash_table_size(ht);
  // printf("\n%d\n", five_entries);
  CU_ASSERT_EQUAL(the_size, five_entries);

  ioopm_hash_table_destroy(ht);
}

void test_ht_remove_str_key(void) {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_to_int, NULL);

  ioopm_hash_table_insert(ht, ptr_elem("4"), ptr_elem("Första"));
  ioopm_hash_table_insert(ht, ptr_elem("72"), ptr_elem("Andra"));
  ioopm_hash_table_insert(ht, ptr_elem("89"), ptr_elem("Tredje"));

  // Remove false entry
  ioopm_option_t false_remove_str = ioopm_hash_table_remove(ht, ptr_elem("21"));
  CU_ASSERT_FALSE(false_remove_str.success);

  // Remove entry that is not the first nor last in the list
  ioopm_option_t remove_middle = ioopm_hash_table_remove(ht, ptr_elem("72"));
  ioopm_option_t struct_test_middle = ioopm_hash_table_lookup(ht, ptr_elem("72"));
  CU_ASSERT_FALSE(struct_test_middle.success);
  CU_ASSERT_TRUE(remove_middle.success);
  
  // Remove the last entry in the list
  ioopm_option_t remove_last = ioopm_hash_table_remove(ht, ptr_elem("89"));
  ioopm_option_t struct_test_last = ioopm_hash_table_lookup(ht, ptr_elem("89"));
  CU_ASSERT_FALSE(struct_test_last.success);
  CU_ASSERT_TRUE(remove_last.success);

  // Remove the first entry in the list
  ioopm_option_t remove_first = ioopm_hash_table_remove(ht, ptr_elem("4"));
  ioopm_option_t struct_test_first = ioopm_hash_table_lookup(ht, ptr_elem("4"));
  CU_ASSERT_FALSE(struct_test_first.success);
  CU_ASSERT_TRUE(remove_first.success);

  // Remove an already removed key
  ioopm_option_t remove_already_removed = ioopm_hash_table_remove(ht, ptr_elem("4"));
  CU_ASSERT_FALSE(remove_already_removed.success);

  ioopm_hash_table_destroy(ht);
}

void test_remove_int_key(void) {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Hej"));
  ioopm_hash_table_insert(ht, int_elem(40), ptr_elem("Hej"));
  ioopm_hash_table_insert(ht, int_elem(57), ptr_elem("Sko"));

  // Remove false entries 
  ioopm_option_t false_remove_int = ioopm_hash_table_remove(ht, int_elem(23));
  ioopm_option_t false_remove_int_v2 = ioopm_hash_table_remove(ht, int_elem(21));
  CU_ASSERT_FALSE(false_remove_int_v2.success);
  CU_ASSERT_FALSE(false_remove_int.success);

  // Remove entry that is not the first nor last in the list
  ioopm_option_t remove_middle = ioopm_hash_table_remove(ht, int_elem(40));
  ioopm_option_t struct_test_middle = ioopm_hash_table_lookup(ht, int_elem(40));
  CU_ASSERT_FALSE(struct_test_middle.success);
  CU_ASSERT_TRUE(remove_middle.success);

  // Remove the last entry in the list
  ioopm_option_t remove_last = ioopm_hash_table_remove(ht, int_elem(57));
  ioopm_option_t struct_test_last = ioopm_hash_table_lookup(ht, int_elem(57));
  CU_ASSERT_FALSE(struct_test_last.success);
  CU_ASSERT_TRUE(remove_last.success);

  // Remove the first entry in the list
  ioopm_option_t remove_first = ioopm_hash_table_remove(ht, int_elem(6));
  ioopm_option_t struct_test_first = ioopm_hash_table_lookup(ht, int_elem(6));
  CU_ASSERT_FALSE(struct_test_first.success);
  CU_ASSERT_TRUE(remove_first.success);

  // Remove key 0
  ioopm_hash_table_insert(ht, int_elem(0), ptr_elem("Apple"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("Apple")));
  CU_ASSERT_EQUAL(1, ioopm_hash_table_size(ht));
  ioopm_option_t remove_zero = ioopm_hash_table_remove(ht, int_elem(0));
  CU_ASSERT_TRUE(remove_zero.success);
  CU_ASSERT_PTR_NULL(ioopm_hash_table_has_value(ht, int_elem(0)));

  // Remove an already removed key
  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(ht));
  ioopm_hash_table_insert(ht, int_elem(-10), ptr_elem("Melon"));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(-10)).success);
  ioopm_option_t remove_already_removed = ioopm_hash_table_remove(ht, int_elem(-10));
  CU_ASSERT_TRUE(remove_already_removed.success);
  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(ht));


  ioopm_hash_table_destroy(ht);
}

void test_hash_table_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  // Check if hashtable without entries is empty
  bool empty = ioopm_hash_table_is_empty(ht);
  CU_ASSERT_TRUE(empty);

  // Check if hashtable with one entry is not empty
  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Hej"));
  bool one_element = ioopm_hash_table_is_empty(ht);
  CU_ASSERT_FALSE(one_element);

  ioopm_hash_table_destroy(ht);
}

void test_hash_table_clear(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  ioopm_hash_table_insert(ht, int_elem(6), ptr_elem("Hej"));
  ioopm_hash_table_clear(ht);
  entry_t *t = ht->buckets[6]->next;
  CU_ASSERT_PTR_NULL(t);

  ioopm_hash_table_destroy(ht);
}

void test_hash_table_keys(void) {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  int keys[5] = {0, 1, 2, 3, 4};
  bool found[5] = {false, false, false, false, false};

  for (int i = 0; i < 5; i++) {
    ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("Hej"));
  }

  ioopm_list_t *linked_list = ioopm_hash_table_keys(ht);
  ioopm_link_t *link = linked_list->first;
  
  // Check for every link in linked_list that it exist in the keys-array
  // If it does, change the boolean on the same index in the found-array
  for (int index = 0; index < 5; index++) {
    for (int j = 0; j < 5; j++) {
      if (link->element.int_value == keys[index]) {
        found[j] = true;
        link = linked_list->first;
        break;
      } else if (j == 4) {
        CU_FAIL("Found a key that was never inserted!");
      } else {
        link = link->next;
      }
    }
  }
  // If all keys were found, we will get true on all asserts
  for (int i = 0; i < 5; i++) {
      CU_ASSERT_TRUE(found[i]);
  }
  ioopm_linked_list_destroy(linked_list);
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_values(void) {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  int keys[5] = {0, 71, 42, 1, 99};
  char *values_arr[5] = {"zero", "ten", "fortytwo", "one", "ninetynine"};

  ioopm_hash_table_insert(ht, int_elem(0), ptr_elem("zero"));
  ioopm_hash_table_insert(ht, int_elem(71), ptr_elem("ten"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("fortytwo"));
  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("one"));
  ioopm_hash_table_insert(ht, int_elem(99), ptr_elem("ninetynine"));

  ioopm_list_t *linked_list = ioopm_hash_table_keys(ht);
  ioopm_link_t *link = linked_list->first;

  ioopm_list_t *ht_values = ioopm_hash_table_values(ht);
  ioopm_link_t *value_itr = ht_values->first;

  // Check for every link in linked_list that it exist in the keys-array
  // If it does, compare respective value with strcmp to see if they are the same
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (link->element.int_value == keys[j]) {
        CU_ASSERT_TRUE(strcmp(value_itr->element.string_value, values_arr[j]) == 0); // Strcmp return 0 if equal
        }
      }
    link = link->next;
    value_itr = value_itr->next;
    }
  
  ioopm_linked_list_destroy(ht_values);
  ioopm_linked_list_destroy(linked_list);
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_has_key(void) {
  // Hashtable for int keys
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  // Hashtable for string keys
  ioopm_hash_table_t *ht_str = ioopm_hash_table_create(string_to_int, value_equiv);

  // Has_key on empty ht
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(53)));
  
  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("three"));
  ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("ten"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("fortytwo"));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(3)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(10)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(53)));
  
  ioopm_hash_table_insert(ht_str, ptr_elem("hej"), ptr_elem("1three"));
  ioopm_hash_table_insert(ht_str, ptr_elem("hallo"), ptr_elem("1ten"));
  ioopm_hash_table_insert(ht_str, ptr_elem("7"), ptr_elem("1fortytwo"));
  
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_str, ptr_elem("hej")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_str, ptr_elem("hallo")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht_str, ptr_elem("8")));

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_destroy(ht_str);
}

void test_hash_table_has_value(void) {
  // Hashtable for string value
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, value_equiv);

  // Hashtable for int values
  ioopm_hash_table_t *ht_int = ioopm_hash_table_create(NULL, NULL);

  // Has_value on exmpty ht
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("fortyone")));
  
  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("three"));
  ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("ten"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("fortytwo"));


  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("three")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("ten")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("fortykone")));

  
  char *copy_three = strdup("three");
  char *copy_ten = strdup("ten");
  char *copy_fortytwo = strdup("fortytwo");

  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(copy_three)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(copy_ten)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(copy_fortytwo)));

  // Values are ints
  // Has_value on empty ht
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht_int, int_elem(4)));
  
  ioopm_hash_table_insert(ht_int, int_elem(3), int_elem(69));
  ioopm_hash_table_insert(ht_int, int_elem(10), int_elem(420));
  ioopm_hash_table_insert(ht_int, int_elem(42), int_elem(696));

  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_int, int_elem(69)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht_int, int_elem(420)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht_int, int_elem(6986)));


  free(copy_three);
  free(copy_ten);
  free(copy_fortytwo);


  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_destroy(ht_int);
}

static void remove_even_key_entries(elem_t key, elem_t value, void *extra) {
  ioopm_hash_table_t *ht = extra;
  if (key.int_value % 2 == 0) {
    ioopm_hash_table_remove(ht, key);
  }
}

void test_hash_table_apply_to_all(void) {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("three"));
  ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("ten"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("fortytwo"));
  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("zero"));
  ioopm_hash_table_insert(ht, int_elem(99), ptr_elem("ninetynine"));

  ioopm_hash_table_apply_to_all(ht, remove_even_key_entries, ht);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(3)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(10)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(42)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(1)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(99)));

  ioopm_hash_table_destroy(ht);
}

bool is_even(elem_t key, elem_t value, void *arg, bool x)
{
    if(key.int_value % 2 == 0) {
        return true;
    } else {
        return false;
    }
}

void test_hash_table_all(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL);

  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, is_even, ht));

  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("three"));
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, is_even, ht))

  ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("ten"));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("fortytwo"));
  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("zero"));
  ioopm_hash_table_insert(ht, int_elem(99), ptr_elem("ninetynine"));

  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, is_even, ht))

  ioopm_hash_table_destroy(ht);
}

int main() {

  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "Test for create_destroy functionality", test_create_destroy) == NULL) || 
    (CU_add_test(my_test_suite, "Test for insert_once functionality", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "Test for size functionality", test_hash_table_size) == NULL) ||
    (CU_add_test(my_test_suite, "Test for remove_str functionality", test_ht_remove_str_key) == NULL) ||
    (CU_add_test(my_test_suite, "Test for remove_int functionality", test_remove_int_key) == NULL) ||
    (CU_add_test(my_test_suite, "Test for empty functionality", test_hash_table_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Test for clear functionality", test_hash_table_clear) == NULL) ||
    (CU_add_test(my_test_suite, "Test for keys functionality", test_hash_table_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Test for values functionality", test_hash_table_values) == NULL) ||
    (CU_add_test(my_test_suite, "Test for has_key functionality", test_hash_table_has_key) == NULL) ||
    (CU_add_test(my_test_suite, "Test for has_value functionality", test_hash_table_has_value) == NULL) ||
    (CU_add_test(my_test_suite, "Test for apply_to_all functionality", test_hash_table_apply_to_all) == NULL) ||
    (CU_add_test(my_test_suite, "Test for all functionality", test_hash_table_all) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
} 