#ifndef REPL_H
#define REPL_H

#include <stdlib.h>

typedef struct {
  char* buf;
  size_t len;
} InputBuffer;

InputBuffer* new_input_buffer();
void read_input(InputBuffer* input_buffer);

#endif