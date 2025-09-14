#include "repl.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buf = NULL;
  input_buffer->len = 0;
  return input_buffer;
}

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&input_buffer->buf, &input_buffer->len, stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->buf[bytes_read - 1] = 0;
}