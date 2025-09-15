#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// add a row to the table
void add_row(Table *table, Row *row) {
  Cursor *cursor = cursor_end(table);
  printf("Inserting at row number: %d\n", table->num_rows);
  void *destination = cursor_location(cursor);
  printf("Destination address: %p\n", destination);

  memcpy(destination, &row->id, sizeof(int));
  memcpy(destination + sizeof(int), row->name, sizeof(row->name));
  memcpy(destination + sizeof(int) + sizeof(row->name), row->email,
         sizeof(row->email));

  table->num_rows += 1;
  free(cursor);
}

void modify_row_by_id(Table *table, Row *row) {
  Cursor *cursor = cursor_at(table, row->id);
  if (cursor == NULL) {
    printf("Error: Row with id %d not found.\n", row->id);
    return;
  }

  cursor_modify_row(cursor, row);

  free(cursor);
  return;
}

void delete_row_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
  if (cursor == NULL) {
    printf("Error: Row with id %d not found.\n", id);
    return;
  }
  cursor_delete_row(cursor);

  return;
}

void print_row_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
  if (cursor == NULL) {
    printf("Error: Row with id %d not found.\n", id);
    return;
  }
  void *row = cursor_location(cursor);
  print_row(row);

  return;
}

// print all rows in the table
void print_all(Table *table) {
  Cursor *cursor = cursor_start(table);

  for (uint32_t i = 0; i < cursor->table->num_rows; i++) {
    void *source = cursor_location(cursor);
    advance_cursor(cursor);
    print_row(source);
  }

  free(cursor);
}

// print a single row
static void print_row(void *source) {
  int id;
  char name[32];
  char email[255];

  memcpy(&id, source, sizeof(int));
  memcpy(&name, source + sizeof(int), sizeof(name));
  memcpy(&email, source + sizeof(int) + sizeof(name), sizeof(email));

  printf("(%d, %s, %s)\n", id, name, email);
}

// Save the table to disk
void save_table(Table *table) {
  printf("Saving table to disk...\n");
  FILE *fp = fopen("../db_file.db", "wb");

  if (fp == NULL) {
    printf("Error: Could not open file for writing.\n");
    return;
  }

  uint32_t full_pages = table->num_rows / PAGE_MAX_ROWS;
  uint32_t extra_rows = table->num_rows % PAGE_MAX_ROWS;

  // Write all full pages
  for (uint32_t i = 0; i < full_pages; i++) {
    if (table->pages[i] != NULL) {
      fwrite(table->pages[i], PAGE_SIZE, 1, fp);
    }
  }

  // Write the last partial page
  if (extra_rows > 0) {
    if (table->pages[full_pages] != NULL) {
      fwrite(table->pages[full_pages], ROW_SIZE, extra_rows, fp);
    }
  }

  fclose(fp);
}

// returns null on failure
Table *load_db() {
  printf("Loading database from disk...\n");
  FILE *fp = fopen("../db_file.db", "rb");

  Table *table = new_table();

  if (fp == NULL) {
    printf("No existing database found. Starting fresh.\n");
    return table;
  }

  // get the file size
  // get the amount of full pages + extra rows in last page
  // start reading the file page by page
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint32_t full_pages = file_size / PAGE_SIZE;
  uint32_t extra_bytes = file_size % PAGE_SIZE;
  uint32_t extra_rows = extra_bytes / ROW_SIZE;

  // Read all full pages
  for (uint32_t i = 0; i < full_pages; i++) {
    table->pages[i] = malloc(PAGE_SIZE);
    fread(table->pages[i], PAGE_SIZE, 1, fp);
    table->num_rows += PAGE_MAX_ROWS;
  }
  // Read the last partial page
  if (extra_rows > 0) {
    table->pages[full_pages] = malloc(PAGE_SIZE);
    if (table->pages[full_pages] == NULL) {
      fclose(fp);
      return NULL;
    }
    fread(table->pages[full_pages], ROW_SIZE, extra_rows, fp);
    table->num_rows += extra_rows;
  }
  fclose(fp);
  return table;
}
