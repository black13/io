#ifndef CBUFFER_H_
#define CBUFFER_H_


// Represents a circular buffer.
#define BUFSIZE 30
typedef struct {
	uint8_t buf[BUFSIZE];
	uint8_t front, back;
} CircularBuffer;

/**
 * Initialize the given circular buffer.
 */
void circular_buffer_init(CircularBuffer* buf);

/**
 * Adds item to the buffer.
 *
 * Call circular_buffer_full to make sure this buffer is not full
 * first.
 */
void circular_buffer_push(CircularBuffer* buf, uint8_t data);

/**
 * Returns the current amount of bytes in the buffer.
 */
uint8_t circular_buffer_size(CircularBuffer* buf);

/**
 * Removes item from the buffer.
 * Returns zero if buffer is empty.
 */
uint8_t circular_buffer_pop(CircularBuffer* buf);

/**
 * Returns the byte at the given POSITIVE offset from the back of the buffer.
 * Returns zero if buffer is empty.
 */
uint8_t circular_buffer_peek(CircularBuffer* buf, int offset);

/**
 * Returns true if circular buffer is full.
 */
bool circular_buffer_full(CircularBuffer* buf);

/**
 * Returns true if circular buffer is empty.
 */
bool circular_buffer_empty(CircularBuffer* buf);


#endif /*CBUFFER_H_*/
