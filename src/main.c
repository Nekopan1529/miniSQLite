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
  Table* table = load_db();

  while (true) {
    printf("sqlite-light >> ");
    read_input(input_buffer);

    Statement statement;
    ProcessResult result = process_input(input_buffer->buf, &statement);

    if (result == PROCESS_SUCCESS) {
      ExecuteResult res = execute_statement(&statement, table);
      if (res == EXECUTE_EXIT) {
        printf("Exiting...\n");
        break;
      } else if (res == EXECUTE_FAIL) {
        printf("Error: Execution failed.\n");
      }
    }
  }

  return 0;
}