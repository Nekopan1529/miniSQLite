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
      memcpy(destination + sizeof(int), statement->row.name,
             sizeof(statement->row.name));
      memcpy(destination + sizeof(int) + sizeof(statement->row.name),
             statement->row.email, sizeof(statement->row.email));
      table->num_rows += 1;

      return EXECUTE_SUCCESS;
    case STATEMENT_SELECT:
      for (uint32_t i = 0; i < table->num_rows; i++) {
        void* source = next_row_slot(table) - ROW_SIZE * (table->num_rows - i);
        int id;
        char name[32];
        char email[255];

        memcpy(&id, source, sizeof(int));
        memcpy(&name, source + sizeof(int), sizeof(name));
        memcpy(&email, source + sizeof(int) + sizeof(name), sizeof(email));

        printf("(%d, %s, %s)\n", id, name, email);
      }
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}
