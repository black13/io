/*
 * uart.h
 *
 *  Created on: Nov 28, 2013
 *      Author: josburn
 */

#ifndef UART_H_
#define UART_H_

#define	RBSZ 1024

typedef struct ringb {
	char	*rb_hd;	  /* head of buffer segment to be read */
	char	*rb_tl;	  /* tail of buffer segment to be written */
	char	rb_buf[RBSZ];	/* segment contents */
} RINGB;

#define	RB_SUCC(rbp, p) \
		((p) >= (rbp)->rb_buf + RBSZ - 1 ? (rbp)->rb_buf : (p) + 1)

#define	RB_ROLLOVER(rbp, p) \
		((p) > (rbp)->rb_buf + RBSZ - 1 ? (rbp)->rb_buf : (p))

#define	RB_PRED(rbp, p) \
		((p) <= (rbp)->rb_buf ? (rbp)->rb_buf + RBSZ - 1 : (p) - 1)

#define	RB_LEN(rp) \
		((rp)->rb_hd <= (rp)->rb_tl ? (rp)->rb_tl - (rp)->rb_hd : \
		RBSZ - ((rp)->rb_hd - (rp)->rb_tl))

#define	RB_CONTIGPUT(rp) \
		(RB_PRED(rp, (rp)->rb_hd) < (rp)->rb_tl ?  \
			(rp)->rb_buf + RBSZ - (rp)->rb_tl : \
			RB_PRED(rp, (rp)->rb_hd) - (rp)->rb_tl)

void UART_Init();

void begin(void);

#endif /* UART_H_ */
