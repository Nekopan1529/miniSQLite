#include "parser.h"

ProcessResult process_input(InputBuffer* input_buffer, Statement* statement) {
  if (strcmp(input_buffer->buf, "insert") == 0) {
    statement->type = STATEMENT_INSERT;
    return PROCESS_SUCCESS;
  } else if (strcmp(input_buffer->buf, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESS_SUCCESS;
  }
  printf("Unrecognized command '%s'\n", input_buffer->buf);
  return PROCESS_UNKNOWN_STATEMENT;
}