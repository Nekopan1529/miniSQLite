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

  } else if (strncmp(input, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    // select <id>
    // if operand is not given, select all
    int assigned = sscanf(input, "select %d", &statement->operand);

    if (assigned < 1) {
      statement->has_operand = false;
      return PROCESS_SUCCESS;
    } else {
      statement->has_operand = true;
    }

    return PROCESS_SUCCESS;

  } else if (strncmp(input, "delete", 6) == 0) {
    statement->type = STATEMENT_DELETE;
    int assigned = sscanf(input, "delete %d", &statement->operand);
    if (assigned < 1) {
      printf("Syntax error. Could not parse id to delete.\n");
      return PROCESS_UNKNOWN_STATEMENT;
    }
    return PROCESS_SUCCESS;

  } else if (strcmp(input, ".exit") == 0) {
    statement->type = STATEMENT_EXIT;
    return PROCESS_SUCCESS;
  } else if (strncmp(input, "modify", 6) == 0) {
    statement->type = STATEMENT_MODIFY;
    int assigned = sscanf(input, "modify %d %s %s", &statement->operand,
                          statement->row.name, statement->row.email);
    if (assigned < 3) {
      printf("Syntax error. Could not parse id to delete.\n");
      return PROCESS_UNKNOWN_STATEMENT;
    }
    return PROCESS_SUCCESS;
  }

  printf("Unrecognized command '%s'\n", input);
  return PROCESS_UNKNOWN_STATEMENT;
}