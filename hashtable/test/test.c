#include "../hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

uint64_t hash(const char *name, size_t length) {
  uint64_t hash_value = 0;
  for (int i = 0; i < length; i++) {
    hash_value += name[i];
    hash_value = hash_value * name[i];
  }
  return hash_value;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Correct Usage: <wordlist filename>\n");
    return EXIT_FAILURE;
  }

  char *filename = argv[1];

  const int tablesize = (1 << 20);
  hash_table *table = hash_table_create(tablesize, hash);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  uint32_t numwords = 0;
  while (!feof(fp) && fgets(buffer, MAX_LINE, fp) != NULL) {
    buffer[strcspn(buffer, "\n\r")] = 0;
    char *newentry = malloc(strlen(buffer) + 1);
    if (newentry == NULL) {
      perror("Failed to create newentry in hash map");
      free(newentry);
      hash_table_destroy(table);
      return EXIT_FAILURE;
    }
    strcpy(newentry, buffer);
    hash_table_insert(table, newentry, strlen(newentry), newentry);
    numwords++;
  }
  fclose(fp);
  printf("Loaded %d words into table.\n", numwords);

  hash_table_destroy(table);
}
