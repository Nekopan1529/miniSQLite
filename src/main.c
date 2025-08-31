#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* buf;
  size_t len;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
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

int main() {
  InputBuffer* input_buffer = new_input_buffer();

  while (true) {
    printf("sqlite-light >> ");

    read_input(input_buffer);
    switch (strcmp(input_buffer->buf, ".exit")) {
      case 0:
        exit(EXIT_SUCCESS);
      default:
        break;
    }
  }
  return 0;
}