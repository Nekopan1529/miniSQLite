#include "pager.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

Pager* pager_open(const char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Unable to open file\n");
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  uint32_t file_length = ftell(fp);

  Pager* pager = malloc(sizeof(Pager));
  pager->fp = fp;
  pager->file_length = file_length;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
}

void* get_page(Pager* pager, uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    printf("Page number out of bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }

  if (pager->pages[page_num] == NULL) {
    // allocate memory
    void* page = malloc(PAGE_SIZE);

    // if within file length, read from disk
    uint32_t num_pages = pager->file_length / PAGE_SIZE;
    if (pager->file_length % PAGE_SIZE) {
      num_pages += 1;
    }

    if (page_num < num_pages) {
      fseek(pager->fp, page_num * PAGE_SIZE, SEEK_SET);
      fread(page, PAGE_SIZE, 1, pager->fp);
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

void pager_flush(Pager* pager, uint32_t page_num) {
  if (pager->pages[page_num] == NULL) {
    printf("Tried to flush null page\n");
    exit(EXIT_FAILURE);
  }

  fseek(pager->fp, page_num * PAGE_SIZE, SEEK_SET);
  fwrite(pager->pages[page_num], PAGE_SIZE, 1, pager->fp);
}

void pager_close(Pager* pager) {
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    if (pager->pages[i] != NULL) {
      pager_flush(pager, i);
      free(pager->pages[i]);
      pager->pages[i] = NULL;
    }
  }

  fclose(pager->fp);
  free(pager);
}
