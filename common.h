#pragma once
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#define No_Buckets 17


#define int_elem(x) (elem_t) { .int_value=(x) }
#define ptr_elem(x) (elem_t) { .string_value=(x) }

typedef union { 
  int   int_value;
  char *string_value;
  void *void_value;
} elem_t;


typedef int (*hash_function)(elem_t key);

/// Compares two elements and returns true if they are equal
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);



// Linked_list
typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file
typedef struct link ioopm_link_t;
typedef struct iter ioopm_list_iterator_t;
typedef bool(*ioopm_int_predicate)(elem_t value, void *extra);
typedef void(*ioopm_apply_int_function)(elem_t *value, void *extra);


struct iter 
{
  ioopm_link_t *current;
  ioopm_list_t *list; /// New field
};

struct link
{
    elem_t element;
    ioopm_link_t *next;
};

struct list
{
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size; // Added field to store the number of elements in the linked list.
    ioopm_eq_function equal_fun;
};


// Hash_table
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *arg, bool extra);
typedef void(*ioopm_apply_function)(elem_t key, elem_t value, void *extra);

#define No_Buckets 17

typedef struct entry entry_t;
typedef struct option ioopm_option_t;
typedef struct hash_table ioopm_hash_table_t;


struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets[No_Buckets];
  hash_function hash_fun;
  ioopm_predicate eq_fun;
};

struct option
{
  bool success;
  elem_t value;
};

