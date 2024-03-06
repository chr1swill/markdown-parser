#include "hashtable.h"
#include "copystring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entry {
  char *key;
  size_t keylen;
  void *object;
  struct entry *next;
} entry;

typedef struct _hash_table {
  uint32_t size;
  hashfunction *hash;
  entry **elements;
} hash_table;

typedef uint64_t(hashfunction)(const char *, size_t);

static size_t hash_table_index(hash_table *ht, const char *key) {
  size_t result = (ht->hash(key, strlen(key)) % ht->size);
  return result;
}

hash_table *hash_table_create(uint32_t size, hashfunction *hf) {
  hash_table *ht = malloc(sizeof(*ht));
  ht->size = size;
  ht->hash = hf;
  // note that calloc zero out all of the memeory
  ht->elements = calloc(sizeof(entry *), ht->size);
  if (ht->elements == NULL) {
    fprintf(stderr,
            "Error: There is not enough space to create a hash table.\n");
    return NULL;
  }

  return ht;
}

void hash_table_destroy(hash_table *ht) {
  for (uint32_t i = 0; i < ht->size; i++) {
    entry *current = ht->elements[i];
    while (current != NULL) {
      entry *to_free = current;
      current = current->next;
      free(to_free->key);
      free(to_free->object);
      free(to_free);
    }
  }
  free(ht->elements);
  free(ht);
}

void hash_table_print(hash_table *ht) {
  printf("Start Table\n");
  for (uint32_t i = 0; i < ht->size; i++) {
    if (ht->elements[i] == NULL) {
      // printf("\t%i\t---\n", i);
    } else {
      printf("\t%i\t\n", i);
      entry *tmp = ht->elements[i];
      while (tmp != NULL) {
        printf("\"%s\"(%p) - ", tmp->key, tmp->object);
        tmp = tmp->next;
      }
      printf("\n");
    }
  }
  printf("End Table\n");
}

bool hash_table_insert(hash_table *ht, const char *key, size_t keylen,
                       void *value_to_insert) {
  if (key == NULL || value_to_insert == NULL || ht == NULL ||
      strlen(value_to_insert) == 0) {
    return false;
  }
  size_t index = hash_table_index(ht, key);

  if (hash_table_lookup(ht, key, strlen(value_to_insert)))
    return false;

  // create new entry
  entry *e = malloc(sizeof(*e));
  if (e == NULL) {
    printf("Failed to allocate memory for new entry\n");
    return false;
  }

  e->object = value_to_insert;
  e->key = malloc(strlen(key) + 1);
  if (e->key == NULL) {
    printf("Failed to allocate memory for new entry key\n");
    return false;
  }

  bool ok = copy_string(e->key, strlen(key) + 1, key);
  if (!ok) {
    free(e->key);
    free(e);
    return false;
  }

  // insert entry
  e->next = ht->elements[index];
  ht->elements[index] = e;
  return true;
}

void *hash_table_lookup(hash_table *ht, const char *key, size_t keylen) {
  if (key == NULL || ht == NULL) {
    return NULL;
  }
  size_t index = hash_table_index(ht, key);

  entry *tmp = ht->elements[index];
  while (tmp != NULL && strcmp(tmp->key, key) != 0) {
    tmp = tmp->next;
  }
  if (tmp == NULL) {
    return NULL;
  }

  return tmp->object;
}

void *hash_table_delete(hash_table *ht, const char *key, size_t keylen) {
  if (key == NULL || ht == NULL) {
    return NULL;
  }
  size_t index = hash_table_index(ht, key);

  entry *tmp = ht->elements[index];
  entry *prev = NULL;
  while (tmp != NULL && strcmp(tmp->key, key) != 0) {
    prev = tmp;
    tmp = tmp->next;
  }
  if (tmp == NULL) {
    return NULL;
  }
  if (prev == NULL) {
    // deleting the head of the list
    ht->elements[index] = tmp->next;
    free(tmp);
  } else {
    // deleting from somewhere not the head
    prev->next = tmp->next;
    free(tmp);
  }

  void *result = tmp->object;
  free(tmp);
  free(prev);
  return result;
}
