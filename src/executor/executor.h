#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../common/statement.h"

typedef enum { EXECUTE_SUCCESS, EXECUTE_FAIL } ExecuteResult;

ExecuteResult execute_statement(Statement* statement);

#endif