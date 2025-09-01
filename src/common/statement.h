#ifndef STATEMENT_H
#define STATEMENT_H

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

#endif