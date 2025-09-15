#ifndef CURSOR_H
#define CURSOR_H
#include <stdbool.h>

#include "size.h"
#include "table.h"

typedef struct {
  Table* table;
  uint32_t row_num;
  bool end_of_table;
} Cursor;

Cursor* cursor_start(Table* table);
Cursor* cursor_end(Table* table);
Cursor* cursor_at(Table* table, int id);
void advance_cursor(Cursor* cursor);
void* cursor_location(Cursor* cursor);
void cursor_delete_row(Cursor* cursor);
void cursor_modify_row(Cursor* cursor, Row* row);

#endif