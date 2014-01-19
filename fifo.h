#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  int n;
  uint8_t elem[16];
  int head, tail;
} fifo_t;

void init_fifo(fifo_t *q);
void free_fifo(fifo_t *q);
int empty_fifo(fifo_t *q);
int add_tail(fifo_t *q, uint8_t item);
int add_head(fifo_t *q, uint8_t item);
int remove_head(fifo_t *q,uint8_t *c);
int remove_tail(fifo_t *q,uint8_t *c);
int num_in_fifo(fifo_t *q);

#endif //  __FIFO_H__
