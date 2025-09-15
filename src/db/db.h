#ifndef DB_H
#define DB_H

#include "../common/statement.h"
#include "cursor.h"
#include "table.h"

void add_row(Table* table, Row* row);
void save_table(Table* table);
void print_all(Table* table);
void print_row_by_id(Table* table, int id);
void delete_row_by_id(Table* table, int id);
void modify_row_by_id(Table* table, Row* row);

static void print_row(void* source);

Table* load_db();

#endif