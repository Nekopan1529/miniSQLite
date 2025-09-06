#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
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

void add_row(Table *table, Statement *statement) {
  void *destination = next_row_slot(table);
  memcpy(destination, &statement->row.id, sizeof(int));
  memcpy(destination + sizeof(int), statement->row.name,
         sizeof(statement->row.name));
  memcpy(destination + sizeof(int) + sizeof(statement->row.name),
         statement->row.email, sizeof(statement->row.email));
  table->num_rows += 1;
}

void print_row(void *source) {
  int id;
  char name[32];
  char email[255];

  memcpy(&id, source, sizeof(int));
  memcpy(&name, source + sizeof(int), sizeof(name));
  memcpy(&email, source + sizeof(int) + sizeof(name), sizeof(email));

  printf("(%d, %s, %s)\n", id, name, email);
}

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