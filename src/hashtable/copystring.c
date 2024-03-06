#include "copystring.h"

bool copy_string(char *destination_for_string, size_t dest_size,
                 const char *source_string_to_copy) {
  // Check for NULL pointers to avoid undefined behavior
  if (destination_for_string == NULL || source_string_to_copy == NULL) {
    fprintf(stderr, "Invalid argument: a NULL pointer was supplied.\n");
    return false;
  }

  size_t source_len = strlen(source_string_to_copy);
  if (source_len + 1 > dest_size) { // +1 for null terminator
    fprintf(stderr,
            "Error: destination is too small or source is too large.\n");
    return false;
  }

  // Use strncpy to copy the string
  strncpy(destination_for_string, source_string_to_copy, dest_size - 1);

  // Ensure the destination is null-terminated
  destination_for_string[dest_size - 1] = '\0';

  // If you reach this point, the operation was successful
  return true;
}
