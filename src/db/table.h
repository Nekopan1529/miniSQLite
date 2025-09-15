#ifndef TABLE_H
#define TABLE_H
#include <stdint.h>

#include "size.h"

typedef struct {
  void* pages[TABLE_MAX_PAGES];
  uint32_t num_rows;
} Table;

Table* new_table();
void* next_row_slot(Table* table);

#endif