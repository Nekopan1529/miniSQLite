#ifndef DB_H
#define DB_H

#include "../common/statement.h"
#include "cursor.h"
#include "pager.h"
#include "table.h"

void db_insert_row(Table* table, Row* row);
void db_save_table(Table* table);
void db_select_all(Table* table);
void db_select_by_id(Table* table, int id);
void db_delete_row_by_id(Table* table, int id);
void db_modify_by_id(Table* table, Row* row);

static void print_row(void* source);

Table* db_load();

#endif