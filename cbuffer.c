
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cbuffer.h"

/**
 * Initialize the given circular buffer.
 */
void circular_buffer_init(CircularBuffer* buf) {
	memset(buf, 0, sizeof(CircularBuffer));
}

/**
 * Adds item to the buffer.
 *
 * Call circular_buffer_full to make sure this buffer is not full
 * first.
 */
void circular_buffer_push(CircularBuffer* buf, uint8_t data) {
	if (circular_buffer_full(buf)) { return; }
	// Increase the UART front position marker accordingly
	buf->buf[buf->front] = data;
	buf->front++;
	if (buf->front >= BUFSIZE) { buf->front = 0; }
}

/**
 * Removes item from the buffer.
 * Returns zero if buffer is empty.
 */
uint8_t circular_buffer_pop(CircularBuffer* buf) {
	if (circular_buffer_empty(buf)) { return 0; }
	uint8_t data = buf->buf[buf->back];
	buf->back++;
	if (buf->back >= BUFSIZE) { buf->back = 0; }
	return data;
}

/**
 * Returns the current amount of bytes in the buffer.
 */
uint8_t circular_buffer_size(CircularBuffer* buf) {
	if (buf->front == buf->back) {
		return 0;
	} else if (buf->front > buf->back) {
		return buf->front - buf->back;
	}
	return buf->front + (BUFSIZE - buf->back);
}

/**
 * Returns the byte at the given POSITIVE offset from the back of the buffer.
 * Returns zero if buffer is empty.
 */
uint8_t circular_buffer_peek(CircularBuffer* buf, int offset) {
	offset += buf->back;
	while (offset >= BUFSIZE) {
		offset -= BUFSIZE;
	}
	return buf->buf[offset];
}

/**
 * Returns true if circular buffer is full.
 */
bool circular_buffer_full(CircularBuffer* buf) {
	return (buf->back == buf->front + 1) ||
	       (buf->front == BUFSIZE - 1 && buf->back == 0);
}

/**
 * Returns true if circular buffer is empty.
 */
bool circular_buffer_empty(CircularBuffer* buf) {
	return buf->front == buf->back;
}
