#ifndef DB_H
#define DB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../common/statement.h"

#define PAGE_SIZE 4096
#define PAGE_MAX_ROWS (PAGE_SIZE / ROW_SIZE)

#define TABLE_MAX_PAGES 100
#define TABLE_MAX_ROWS (TABLE_MAX_PAGES * PAGE_SIZE) / ROW_SIZE

#define ROW_SIZE sizeof(Row)
#define ROW_ID_SIZE sizeof(((Row*)0)->id)
#define ROW_NAME_SIZE sizeof(((Row*)0)->name)
#define ROW_EMAIL_SIZE sizeof(((Row*)0)->email)

typedef struct {
  void* pages[TABLE_MAX_PAGES];
  uint32_t num_rows;
} Table;

Table* new_table();
void* next_row_slot(Table* table);
void add_row(Table* table, Row* row);
void save_table(Table* table);
void print_all(Table* table);
void print_row_by_id(Table* table, int id);

typedef struct {
  Table* table;
  uint32_t row_num;
  bool end_of_table;
} Cursor;

Cursor* cursor_start(Table* table);
Cursor* cursor_end(Table* table);
static Cursor* cursor_at(Table* table, int id);
static void advance_cursor(Cursor* cursor);
static void* cursor_location(Cursor* cursor);
void cursor_delete_row(Cursor* cursor);

static void print_row(void* source);

Table* load_db();

#endif