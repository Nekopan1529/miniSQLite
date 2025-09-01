#include "db.h"

#include <stdlib.h>

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->num_rows = 0;
  return table;
}

// returns the index of where to put row next in the page.
void *next_row_slot(Table *table) {
  uint32_t row_offset = table->num_rows % TABLE_MAX_ROWS;
  uint32_t page_num = table->num_rows / TABLE_MAX_ROWS;

  // allocate page if it doesn't exist
  if (table->pages[page_num] == NULL) {
    table->pages[page_num] = malloc(PAGE_SIZE);
  }

  // pointer to the row slot
  void *page = table->pages[page_num];
  return (char *)page + row_offset * ROW_SIZE;
}