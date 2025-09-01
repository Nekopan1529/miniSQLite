#ifndef STATEMENT_H
#define STATEMENT_H

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

// this is a temporary hard coded statement for making the database part work
typedef struct {
  StatementType type;
  int id;
  char name[32];
  char email[255];
} Statement;

#endif