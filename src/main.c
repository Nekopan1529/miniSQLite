#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* buf;
  size_t len;
} InputBuffer;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef enum { PROCESS_SUCCESS, PROCESS_UNKNOWN_STATEMENT } ProcessResult;

typedef enum { EXECUTE_SUCCESS, EXECUTE_FAIL } ExecuteResult;

typedef struct {
  StatementType type;
} Statement;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  return input_buffer;
}

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&input_buffer->buf, &input_buffer->len, stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->buf[bytes_read - 1] = 0;
}

ProcessResult process_input(InputBuffer* input_buffer, Statement* statement) {
  if (strcmp(input_buffer->buf, "insert") == 0) {
    statement->type = STATEMENT_INSERT;
    return PROCESS_SUCCESS;
  } else if (strcmp(input_buffer->buf, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESS_SUCCESS;
  }
  printf("Unrecognized command '%s'\n", input_buffer->buf);
  return PROCESS_UNKNOWN_STATEMENT;
}

ExecuteResult execute_statement(InputBuffer* input_buffer,
                                Statement* statement) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      printf("This is where we would do an insert.\n");
      return EXECUTE_SUCCESS;
    case STATEMENT_SELECT:
      printf("This is where we would do a select.\n");
      return EXECUTE_SUCCESS;
    default:
      return EXECUTE_FAIL;
  }
}

int main() {
  InputBuffer* input_buffer = new_input_buffer();

  while (true) {
    printf("sqlite-light >> ");
    read_input(input_buffer);

    Statement statement;
    ProcessResult result = process_input(input_buffer, &statement);

    if (result == PROCESS_SUCCESS) {
      execute_statement(input_buffer, &statement);
    }
  }
  return 0;
}