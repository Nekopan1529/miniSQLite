#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "repl/repl.h"

typedef enum { EXECUTE_SUCCESS, EXECUTE_FAIL } ExecuteResult;

ExecuteResult execute_statement(InputBuffer* input_buffer,
                                Statement* statement) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      printf("This is where we would do an insert.\n");
      return EXECUTE_SUCCESS;
    case STATEMENT_SELECT:
      printf("This is where we would do a select.\n");
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}

int main() {
  InputBuffer* input_buffer = new_input_buffer();

  while (true) {
    printf("sqlite-light >> ");
    read_input(input_buffer);

    Statement statement;
    ProcessResult result = process_input(input_buffer, &statement);

    if (result == PROCESS_SUCCESS) {
      execute_statement(input_buffer, &statement);
    }
  }
  return 0;
}