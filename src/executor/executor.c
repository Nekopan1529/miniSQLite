#include "executor.h"

#include <stdio.h>

#include "../common/statement.h"
#include "../db/db.h"

ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      table->rows[table->num_rows] = statement->row;
      table->num_rows += 1;
      return EXECUTE_SUCCESS;
    case STATEMENT_SELECT:
      printf("This is where we would do a select.\n");
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}
