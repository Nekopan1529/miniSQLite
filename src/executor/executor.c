#include "executor.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/statement.h"
#include "../db/db.h"

ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      if (table->num_rows >= TABLE_MAX_ROWS) {
        printf("Error: Table full.\n");
        return EXECUTE_FAIL;
      }
      add_row(table, &statement->row);
      return EXECUTE_SUCCESS;

    case STATEMENT_SELECT:
      print_all(table);
      return EXECUTE_SUCCESS;
    case STATEMENT_DELETE:

    case STATEMENT_EXIT:

      save_table(table);
      exit(EXIT_SUCCESS);
      return EXECUTE_SUCCESS;

    default:
      return EXECUTE_FAIL;
  }
}
