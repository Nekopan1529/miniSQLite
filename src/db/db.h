#ifndef DB_H
#define DB_H

#include <stddef.h>
#include <stdint.h>

#include "../common/statement.h"

// for the paging structure
#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100

#define ROW_SIZE sizeof(Row)
#define PAGE_MAX_ROWS (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS (TABLE_MAX_PAGES * PAGE_SIZE) / ROW_SIZE

typedef struct {
  void* pages[TABLE_MAX_PAGES];
  uint32_t num_rows;
} Table;

Table* new_table();
void* next_row_slot(Table* table);
void add_row(Table* table, Row* row);
void print_row(void* source);
void save_table(Table* table);
Table* load_db();

#endif