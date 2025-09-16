#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// add a row to the table
void db_insert_row(Table *table, Row *row) {
  Cursor *cursor = cursor_end(table);
  void *destination = cursor_location(cursor);  // uses get_page internally

  memcpy(destination, &row->id, ROW_ID_SIZE);
  memcpy(destination + ROW_ID_SIZE, row->name, ROW_NAME_SIZE);
  memcpy(destination + ROW_ID_SIZE + ROW_NAME_SIZE, row->email, ROW_EMAIL_SIZE);

  table->num_rows += 1;
  free(cursor);
}

void db_modify_by_id(Table *table, Row *row) {
  Cursor *cursor = cursor_at(table, row->id);
  if (!cursor) {
    printf("Error: Row with id %d not found.\n", row->id);
    return;
  }

  cursor_modify_row(cursor, row);  // uses cursor_location internally
  free(cursor);
}

void db_delete_row_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
  if (!cursor) {
    printf("Error: Row with id %d not found.\n", id);
    return;
  }

  cursor_delete_row(cursor);  // uses cursor_location internally
  free(cursor);
}

void db_select_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
  if (!cursor) {
    printf("Error: Row with id %d not found.\n", id);
    return;
  }

  void *row = cursor_location(cursor);
  print_row(row);
  free(cursor);
}

// print all rows in the table
void db_select_all(Table *table) {
  Cursor *cursor = cursor_start(table);

  while (!cursor->end_of_table) {
    void *source = cursor_location(cursor);
    print_row(source);
    cursor_advance(cursor);
  }

  free(cursor);
}

// print a single row
static void print_row(void *source) {
  int id;
  char name[ROW_NAME_SIZE + 1];    // +1 for '\0'
  char email[ROW_EMAIL_SIZE + 1];  // +1 for '\0'

  memcpy(&id, source, ROW_ID_SIZE);
  memcpy(name, source + ROW_ID_SIZE, ROW_NAME_SIZE);
  name[ROW_NAME_SIZE] = '\0';  // ensure null-termination

  memcpy(email, source + ROW_ID_SIZE + ROW_NAME_SIZE, ROW_EMAIL_SIZE);
  email[ROW_EMAIL_SIZE] = '\0';  // ensure null-termination

  printf("(%d, %s, %s)\n", id, name, email);
}

// Save the table to disk
void db_save_table(Table *table) {
  printf("Saving table to disk...\n");
  table_save(table);
}

// returns null on failure
Table *db_load() {
  printf("Loading database from disk...\n");
  return table_load();
}
