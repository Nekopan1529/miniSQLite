#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <string.h>

#include "../common/statement.h"
#include "../repl/repl.h"

typedef enum { PROCESS_SUCCESS, PROCESS_UNKNOWN_STATEMENT } ProcessResult;

ProcessResult process_input(InputBuffer* input_buffer, Statement* statement);

#endif