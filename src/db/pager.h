#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "size.h"

typedef struct {
  FILE* fp;
  uint32_t file_length;
  void* pages[TABLE_MAX_PAGES];
} Pager;

Pager* pager_open(const char* filename);

#endif