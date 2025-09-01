#include "executor.h"

#include <memory.h>
#include <stdio.h>

#include "../common/statement.h"
#include "../db/db.h"

ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      if (table->num_rows >= TABLE_MAX_ROWS) {
        printf("Error: Table full.\n");
        return EXECUTE_FAIL;
      }

      // serialize row
      void* destination = next_row_slot(table);
      memcpy(destination, &statement->row.id, sizeof(int));
      memcpy(destination + sizeof(int), &statement->row.name,
             sizeof(&statement->row.name));
      memcpy(destination + sizeof(int) + sizeof(&statement->row.name),
             &statement->row.email, sizeof(&statement->row.email));
      // use next_row_slot to get the pointer to where to put the row
      //  add the row to void pointer in table

      return EXECUTE_SUCCESS;
    case STATEMENT_SELECT:
      /*    for (size_t i = 0; i < table->num_rows; i++) {
            Row row = table->rows[i];
            printf("%d %s %s\n", row.id, row.name, row.email);
          } */
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}
