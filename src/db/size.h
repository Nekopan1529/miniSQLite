#ifndef SIZE_H
#define SIZE_H

#include "../common/statement.h"

#define PAGE_SIZE 4096
#define PAGE_MAX_ROWS (PAGE_SIZE / ROW_SIZE)

#define TABLE_MAX_PAGES 100
#define TABLE_MAX_ROWS (TABLE_MAX_PAGES * PAGE_SIZE) / ROW_SIZE

#define ROW_SIZE sizeof(Row)
#define ROW_ID_SIZE sizeof(((Row*)0)->id)
#define ROW_NAME_SIZE sizeof(((Row*)0)->name)
#define ROW_EMAIL_SIZE sizeof(((Row*)0)->email)

#endif