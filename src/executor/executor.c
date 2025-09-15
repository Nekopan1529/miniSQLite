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
      db_insert_row(table, &statement->row);
      return EXECUTE_SUCCESS;

    case STATEMENT_SELECT:
      if (statement->has_operand == false) {
        db_select_all(table);
      } else {
        db_select_by_id(table, statement->operand);
      }

      return EXECUTE_SUCCESS;
    case STATEMENT_DELETE:
      db_delete_row_by_id(table, statement->operand);
      return EXECUTE_SUCCESS;
      break;

    case STATEMENT_EXIT:

      db_save_table(table);
      exit(EXIT_SUCCESS);
      return EXECUTE_SUCCESS;
    case STATEMENT_MODIFY:
      Row* row = (Row*)malloc(sizeof(Row));
      row->id = statement->operand;
      memcpy(row->name, statement->row.name, sizeof(statement->row.name));
      memcpy(row->email, statement->row.email, sizeof(statement->row.email));

      // modifies the row in the table with the id in row
      // changes that row to row->name and row->email
      db_modify_by_id(table, row);
      free(row);
      return EXECUTE_SUCCESS;

    default:
      return EXECUTE_FAIL;
  }
}
