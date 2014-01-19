
#include "fifo.h"

void init_fifo(fifo *q)
{
  //n++;                                /* one bigger than necessary */
  //q->elem = malloc(n * sizeof(*(q->elem)));
  //assert(q->elem);
  q->n = 16;//n;
  q->head = q->tail = 0;
}

void free_fifo(fifo *q)
{
  free(q->elem);
  //q->elem = 0;
  q->n = 0;
  q->head = q->tail = 0;
}

int empty_fifo(fifo *q)
{
  return (q->head == q->tail);
}

int add_tail(fifo *q, uint8_t item)
{
	int new_tail = (q->tail + 1) % (q->n);
	if (new_tail == q->head)
	{
		return 0;
	}
	else
	{
		q->elem[q->tail] = item;
		q->tail = new_tail;
		return 1;
	}
}

int add_head(fifo *q, uint8_t item)
{
	int new_head = q->head - 1;
	if (new_head < 0)
	{
		new_head += q->n;
	}
	if (new_head == q->tail)
	{
		return 0;
	}
	else
	{
		q->elem[new_head] = item;
		q->head = new_head;
		return 1;
	}
}

int remove_head(fifo *q,uint8_t *c)
{
	//uint8_t item;
	if (empty_fifo(q))
	{
		return 0;
	}
	*c = q->elem[q->head];
	q->head = (q->head + 1) % (q->n);
	return 1;
}

int remove_tail(fifo *q,uint8_t *c)
{
	int new_tail = q->tail - 1;
	if (empty_fifo(q))
	{
		return 0;
	}
	if (new_tail < 0)
	{
		new_tail += q->n;
	}
	q->tail = new_tail;
	*c = q->elem[q->tail];
	return 1;
}

int num_in_fifo(fifo *q)
{
	int res = q->tail - q->head;
	return (res >= 0) ? res : res + q->n;
}

