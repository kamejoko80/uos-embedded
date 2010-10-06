#ifndef __CAN_H_
#define __CAN_H_

/*
 * Stack size for the task of CAN interrupt handler.
*/
#ifndef CAN_STACKSZ
#   define CAN_STACKSZ		1000
#endif

/*
 * CAN data packet.
 */
typedef struct __attribute__((__packed__)) _can_frame_t {
	unsigned id;
#define	CAN_ID_EID_SHIFT	0		/* Сдвиг поля EID */
#define	CAN_ID_EID_MASK		0x3FFFF		/* Маска поля EID */
#define	CAN_ID_SID_SHIFT	18		/* Сдвиг поля SID */
#define	CAN_ID_SID_MASK		(0x7FF << 18)	/* Маска поля SID */

	unsigned dlc;
#define	CAN_DLC_LEN(n)		((n) & 15)	/* Длина данных в байтах */
#define	CAN_DLC_RTR		(1 << 8)	/* Запрос обратного ответа */
#define	CAN_DLC_IDE		(1 << 12)	/* Расширенный формат */

	unsigned data [2];			/* До 8 байт данных */
} can_frame_t;

/*
 * Queue of CAN packets.
 */
#define CAN_QUEUE_SIZE		16	/* max number of packets in queue */

typedef struct _can_queue_t {
	unsigned count;
	can_frame_t *tail;
	can_frame_t queue [CAN_QUEUE_SIZE];
} can_queue_t;

/*
 * Data structure for CAN channel.
 */
typedef struct _can_t {
	mutex_t lock;			/* interrupt goes here */

	unsigned port;			/* port number */
	unsigned kbitsec;		/* data rate kbit/sec */
	int loop;			/* enable internal loopback */
	can_queue_t inq;		/* queue of received packets */

	/* Statistics. */
	unsigned long intr;		/* interrupt counter */
	unsigned rec, tec;		/* hardware rx/tx error counters */

	unsigned long in_packets;	/* received packets */
	unsigned long in_errors;	/* receive errors */
	unsigned long in_frame_errors;	/* receive frame errors */
	unsigned long in_crc_errors;	/* receive CRC errors */
	unsigned long in_bitstuff_errors; /* receive bit-stuff errors */
	unsigned long in_discards;	/* ignored packets, due to lack of memory */

	unsigned long out_packets;	/* transmitted packets */
	unsigned long out_errors;	/* transmit errors */
	unsigned long out_collisions;	/* lost arbitrage */
	unsigned long out_nacks;	/* no ack */
	unsigned long out_bit_errors;	/* transmit bit errors */

	ARRAY (stack, CAN_STACKSZ);	 /* task stack */
} can_t;

/*
 * User level API.
 */
void can_init (can_t *c, int port, int prio, unsigned kbitsec);
void can_set_speed (can_t *c, unsigned kbitsec);
void can_set_loop (can_t *c, int on);
void can_output (can_t *c, const can_frame_t *fr);
void can_input (can_t *c, can_frame_t *fr);

#endif /* !__CAN_H_ */