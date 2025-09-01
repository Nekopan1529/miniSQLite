#include "executor.h"

#include "../repl/repl.h"

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
