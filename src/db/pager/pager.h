#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "../storage/size.h"

typedef struct {
  FILE* fp;
  uint32_t file_length;
  void* pages[TABLE_MAX_PAGES];
} Pager;

Pager* pager_open(const char* filename);
void pager_close(Pager* pager, uint32_t num_rows);
void pager_flush(Pager* pager, uint32_t page_num, uint32_t num_rows);
void* pager_get_page(Pager* pager, uint32_t page_num);

#endif