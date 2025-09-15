#include "pager.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

Pager* pager_open(const char* filename) {
  FILE* fp = fopen(filename, "r+b");
  if (!fp) {
    fp = fopen(filename, "w+b");
    if (!fp) {
      perror("Unable to create file");
      exit(EXIT_FAILURE);
    }
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

/* checks if the page is in memory: If yes → return it. If no → allocate memory,
 read from disk (or zero it if the page is new), then return it. */
void* pager_get_page(Pager* pager, uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    printf("Page number out of bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }

  if (pager->pages[page_num] == NULL) {
    void* page = malloc(PAGE_SIZE);
    // zero by default
    memset(page, 0, PAGE_SIZE);

    // Determine number of pages in the file
    uint32_t num_pages = pager->file_length / PAGE_SIZE;
    if (pager->file_length % PAGE_SIZE) num_pages++;

    if (page_num < num_pages) {
      fseek(pager->fp, page_num * PAGE_SIZE, SEEK_SET);
      size_t bytes_read = fread(page, 1, PAGE_SIZE, pager->fp);

      // For last partial page, zero the rest
      if (bytes_read < PAGE_SIZE) {
        memset((char*)page + bytes_read, 0, PAGE_SIZE - bytes_read);
      }
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

/*When you want to save changes, the pager writes pages back to disk using
pager_flush().

Only the parts of the page that actually contain rows need to be saved (last
partial page might not be full).*/
void pager_flush(Pager* pager, uint32_t page_num, uint32_t num_rows) {
  if (pager->pages[page_num] == NULL) {
    printf("Tried to flush null page\n");
    exit(EXIT_FAILURE);
  }

  fseek(pager->fp, page_num * PAGE_SIZE, SEEK_SET);

  // Calculate how many bytes to write
  uint32_t rows_in_page = PAGE_MAX_ROWS;
  if ((page_num + 1) * PAGE_MAX_ROWS > num_rows) {
    rows_in_page = num_rows % PAGE_MAX_ROWS;
  }

  size_t bytes_to_write = rows_in_page * ROW_SIZE;
  if (bytes_to_write == 0) bytes_to_write = PAGE_SIZE;  // full page

  fwrite(pager->pages[page_num], 1, bytes_to_write, pager->fp);
}

void pager_close(Pager* pager, uint32_t num_rows) {
  uint32_t total_pages = (num_rows + PAGE_MAX_ROWS - 1) / PAGE_MAX_ROWS;

  for (uint32_t i = 0; i < total_pages; i++) {
    if (pager->pages[i] != NULL) {
      pager_flush(pager, i, num_rows);
      free(pager->pages[i]);
      pager->pages[i] = NULL;
    }
  }

  fclose(pager->fp);
  free(pager);
}
