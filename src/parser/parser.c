#include "parser.h"

#include <stdio.h>
#include <string.h>

ProcessResult process_input(char* input, Statement* statement) {
  // 6 because insert is 6 chars
  if (strncmp(input, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    // for the hard coded table of a user, the syntax of inser is
    // insert <id> <name> <email>
    int assigned = sscanf(input, "insert %d %s %s", &statement->row.id,
                          statement->row.name, statement->row.email);
    if (assigned < 3) {
      printf("Syntax error. Could not parse all fields.\n");
      return PROCESS_UNKNOWN_STATEMENT;
    }

    return PROCESS_SUCCESS;
  } else if (strcmp(input, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESS_SUCCESS;
  }
  printf("Unrecognized command '%s'\n", input);
  return PROCESS_UNKNOWN_STATEMENT;
}