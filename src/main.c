#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db/db.h"
#include "executor/executor.h"
#include "parser/parser.h"
#include "repl/repl.h"

int main() {
  InputBuffer* input_buffer = new_input_buffer();
  Table* table = new_table();

  while (true) {
    printf("sqlite-light >> ");
    read_input(input_buffer);

    Statement statement;
    ProcessResult result = process_input(input_buffer->buf, &statement);

    if (result == PROCESS_SUCCESS) {
      execute_statement(&statement, table);
    }
  }
  return 0;
}