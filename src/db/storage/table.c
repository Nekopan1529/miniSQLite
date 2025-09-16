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

Table *table_load() {
  // Open the pager (will create file if it doesn't exist)
  Pager *pager = pager_open("../db_file.db");
  if (!pager) {
    perror("Failed to open database file");
    exit(EXIT_FAILURE);
  }

  // Create table and attach pager
  Table *table = table_new_table();
  table->pager = pager;

  // Calculate number of rows from file length
  table->num_rows = pager->file_length / ROW_SIZE;

  return table;
}

void table_save(Table *table) {
  if (!table || !table->pager) return;

  // pager_close now flushes all pages to disk
  pager_close(table->pager, table->num_rows);

  table->pager = NULL;  // avoid dangling pointer
}