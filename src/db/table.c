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