#include "table.h"

#include <stddef.h>
#include <stdlib.h>

// create a new table
Table *table_new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->pages[0] = NULL;
  table->num_rows = 0;
  return table;
}

// returns the index of where to put row next in the page.
void *next_row_slot(Table *table) {
  uint32_t rows_in_page = table->num_rows % PAGE_MAX_ROWS;
  uint32_t page_num = table->num_rows / PAGE_MAX_ROWS;

  // allocate page if it doesn't exist
  if (table->pages[page_num] == NULL) {
    table->pages[page_num] = malloc(PAGE_SIZE);
  }

  // pointer to the row slot
  void *page = table->pages[page_num];
  return (char *)page + rows_in_page * ROW_SIZE;
}