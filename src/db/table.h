#ifndef TABLE_H
#define TABLE_H
#include <stdint.h>

#include "size.h"

typedef struct {
  void* pages[TABLE_MAX_PAGES];
  uint32_t num_rows;
} Table;

Table* table_new_table();

#endif