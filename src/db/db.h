#ifndef DB_H
#define DB_H

#include <stddef.h>

#include "../common/statement.h"

#define MAX_ROWS 1000

typedef struct {
  Row rows[MAX_ROWS];
  size_t num_rows;
} Table;

#endif