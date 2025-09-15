#include "table.h"

#include <stddef.h>
#include <stdlib.h>

// create a new table
Table *table_new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->num_rows = 0;
  return table;
}

void table_free(Table *table) {
  if (table == NULL) return;

  if (table->pager != NULL) {
    pager_close(table->pager, table->num_rows);
    table->pager = NULL;
  }

  free(table);
}