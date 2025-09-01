#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../common/statement.h"
#include "../db/db.h"

typedef enum { EXECUTE_SUCCESS, EXECUTE_FAIL } ExecuteResult;

ExecuteResult execute_statement(Statement* statement, Table* table);

#endif