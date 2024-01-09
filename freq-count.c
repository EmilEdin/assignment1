#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"


#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"
int count = 0;
int string_sum_hash(elem_t e)
{
  char *str = e.string_value;
  int result = 0;
  do
  {
    result += *str;
  } while (*++str != '\0');
  return result;
}

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  // FIXME: Rewrite to match your own interface, error-handling, etc.
  int freq =
      ioopm_hash_table_has_key(ht, ptr_elem(word)) ? (ioopm_hash_table_lookup(ht, ptr_elem(word))).value.int_value : 0;
  
  if (freq >= 1)
  {
    ioopm_hash_table_insert(ht, ptr_elem(word), int_elem(freq + 1));
  }
  else
  {
    ioopm_hash_table_insert(ht, ptr_elem(strdup(word)), int_elem(freq + 1));
  }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f))
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }

    free(buf);
  }

  fclose(f);
}



bool string_eq(elem_t e1, elem_t e2, void *arg, bool x)
{
  return (strcmp(e1.string_value, e2.string_value) == 0);
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, NULL);

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    // FIXME: If the keys are returned as a list, transfer them into
    // an array to use `sort_keys` (perhaps using an iterator?)
    ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
    

    ioopm_list_t *keys_buf = keys_list;
    char **keys = calloc(1, sizeof(char *) * ioopm_linked_list_size(keys_list));

    ioopm_link_t *keys_link = keys_list->first;
    int counter = 0;
    while (keys_link != NULL)
    {
      keys[counter] = keys_link->element.string_value;
      counter = counter + 1;
      keys_link = keys_link->next;
    }

    int size = ioopm_hash_table_size(ht);
    sort_keys(keys, size);

    for (int i = 0; i < size; ++i)
    {
      int freq = (ioopm_hash_table_lookup(ht, ptr_elem(keys[i]))).value.int_value;
      printf("%s: %d\n", keys[i], freq);
      
    }
   
  

   
    for (int i = 0; i < keys_list->size; ++i)
    {
      free(keys[i]);
    }

    free(keys);
    ioopm_linked_list_destroy(keys_buf);
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }

  ioopm_hash_table_destroy(ht);
}
