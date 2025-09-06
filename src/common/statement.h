#ifndef STATEMENT_H
#define STATEMENT_H

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
  STATEMENT_EXIT
} StatementType;

// this is a temporary hard coded statement for making the database part work

typedef struct {
  int id;
  char name[32];
  char email[255];
} Row;

typedef struct {
  StatementType type;
  Row row;
} Statement;

#endif