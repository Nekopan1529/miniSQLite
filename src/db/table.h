#ifndef TABLE_H
#define TABLE_H
#include <stdint.h>

#include "pager.h"
#include "size.h"

typedef struct {
  Pager* pager;
  uint32_t num_rows;
} Table;

Table* table_new_table();
void table_free(Table* table);

#endif