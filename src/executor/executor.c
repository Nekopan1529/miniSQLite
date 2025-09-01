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
      for (size_t i = 0; i < table->num_rows; i++) {
        Row row = table->rows[i];
        printf("%d %s %s\n", row.id, row.name, row.email);
      }
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}
