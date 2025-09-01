#include "parser.h"

#include <stdio.h>
#include <string.h>

ProcessResult process_input(char* input, Statement* statement) {
  if (strcmp(input, "insert") == 0) {
    statement->type = STATEMENT_INSERT;
    return PROCESS_SUCCESS;
  } else if (strcmp(input, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESS_SUCCESS;
  }
  printf("Unrecognized command '%s'\n", input);
  return PROCESS_UNKNOWN_STATEMENT;
}