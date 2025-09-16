#include "cursor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../pager/pager.h"

// create a cursor at the start of the table
Cursor *cursor_start(Table *table) {
  Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = 0;
  cursor->end_of_table = (table->num_rows == 0);
  return cursor;
}

// create a cursor at the end of the table
Cursor *cursor_end(Table *table) {
  Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = table->num_rows;
  cursor->end_of_table = true;
  return cursor;
}

// return a cursor at the row with the given id. Return NULL if not found
Cursor *cursor_at(Table *table, int target_id) {
  Cursor *cursor = cursor_start(table);

  for (uint32_t i = 0; i < cursor->table->num_rows; i++) {
    void *source = cursor_location(cursor);
    int tmp_id;
    memcpy(&tmp_id, source, sizeof(int));

    if (tmp_id == target_id) {
      return cursor;
    }

    cursor_advance(cursor);
  }
  return NULL;
}

// advance the cursor to the next row
void cursor_advance(Cursor *cursor) {
  cursor->row_num += 1;
  if (cursor->row_num >= cursor->table->num_rows) {
    cursor->end_of_table = true;
  }
}

// return the memory address of the row at the cursor position
void *cursor_location(Cursor *cursor) {
  uint32_t row = cursor->row_num;
  if (row > cursor->table->num_rows) {
    printf("Error: Cursor out of bounds.\n");
    exit(EXIT_FAILURE);
  }

  uint32_t page_num = row / PAGE_MAX_ROWS;
  uint32_t row_offset = row % PAGE_MAX_ROWS;

  // Use pager to get the page, lazily allocating if needed
  void *page = pager_get_page(cursor->table->pager, page_num);

  return (char *)page + row_offset * ROW_SIZE;
}

// delete the row at the cursor position
void cursor_delete_row(Cursor *cursor) {
  void *rowptr = cursor_location(cursor);
  memset(rowptr, 0, ROW_SIZE);

  cursor->table->num_rows -= 1;
  bool end_of_table = (cursor->row_num >= cursor->table->num_rows);

  if (end_of_table) {
    cursor->end_of_table = end_of_table;
    // if we deleted the last row, just move the cursor to the end
    cursor->row_num = cursor->table->num_rows;

  } else {
    Cursor *end = cursor_end(cursor->table);
    void *last_row = cursor_location(end);
    void *empty_row = cursor_location(cursor);

    size_t row_size = (ROW_ID_SIZE + ROW_NAME_SIZE + ROW_EMAIL_SIZE);
    memcpy(empty_row, last_row, row_size);
    memset(last_row, 0, row_size);
    free(end);
  }
}

void cursor_modify_row(Cursor *cursor, Row *row) {
  void *location = cursor_location(cursor);
  memcpy(location + ROW_ID_SIZE, row->name, ROW_NAME_SIZE);
  memcpy(location + ROW_ID_SIZE + ROW_NAME_SIZE, row->email, ROW_EMAIL_SIZE);
  return;
}
