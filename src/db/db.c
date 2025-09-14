#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create a new table
Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->pages[0] = NULL;
  table->num_rows = 0;
  return table;
}

// returns the index of where to put row next in the page.
void *next_row_slot(Table *table) {
  uint32_t rows_in_page = table->num_rows % PAGE_MAX_ROWS;
  uint32_t page_num = table->num_rows / PAGE_MAX_ROWS;

  // allocate page if it doesn't exist
  if (table->pages[page_num] == NULL) {
    table->pages[page_num] = malloc(PAGE_SIZE);
  }

  // pointer to the row slot
  void *page = table->pages[page_num];
  return (char *)page + rows_in_page * ROW_SIZE;
}

// return the memory address of the row at the cursor position
static void *cursor_location(Cursor *cursor) {
  uint32_t row = cursor->row_num;
  if (row > cursor->table->num_rows) {
    printf("Error: Cursor out of bounds.\n");
    exit(EXIT_FAILURE);
  }
  uint32_t page_num = row / PAGE_MAX_ROWS;
  uint32_t row_offset = row % PAGE_MAX_ROWS;
  void *page = cursor->table->pages[page_num];

  if (page == NULL) {
    // Allocate page lazily
    page = malloc(PAGE_SIZE);
    cursor->table->pages[page_num] = page;
  }

  return (char *)page + row_offset * ROW_SIZE;
}

// advance the cursor to the next row
static void advance_cursor(Cursor *cursor) {
  cursor->row_num += 1;
  if (cursor->row_num >= cursor->table->num_rows) {
    cursor->end_of_table = true;
  }
}

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
  }
}

// return a cursor at the row with the given id
static Cursor *cursor_at(Table *table, int target_id) {
  Cursor *cursor = cursor_start(table);

  for (uint32_t i = 0; i < cursor->table->num_rows; i++) {
    void *source = cursor_location(cursor);
    int tmp_id;
    memcpy(&tmp_id, source, sizeof(int));

    if (tmp_id == target_id) {
      break;
    }

    advance_cursor(cursor);
  }
  return cursor;
}

void delete_row_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
  cursor_delete_row(cursor);

  return;
}

void print_row_by_id(Table *table, int id) {
  Cursor *cursor = cursor_at(table, id);
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