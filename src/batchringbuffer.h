#ifndef _BATCH_RING_BUFFER_H_
#define _BATCH_RING_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "lockfree_shared.h"

typedef enum {
	AVAILABLE = 1, 
	WRITING = 2,
	CANCELED = 3,
	PUBLISHED = 4,
	DIRTY = 5,
} brb_batch_states;

typedef struct brb_batch {
	/** <  The counter used to & to find the index of this batch in the buffer */
	volatile uint64_t				batch_num;
	/** <  How many entries to claim for this batch */
	volatile uint64_t 				batch_size;
	/** <  Seq number assigend to this batch */
	volatile uint64_t 				seq_num;
	/** <  Padding to keep other data elements out of this cache line */
	volatile uint64_t				data[5];					
	/** <  Current state of the batch */
	volatile brb_batch_states	 	state 				____cacheline_aligned;
} brb_batch;

typedef struct brb_buffer brb_buffer;

#define BRB_SUCCESS 					0 						/** Successful result */
#define BRB_ERROR						(RB_ERROR - 5000)				/** Generic error */

#define BRB_ALLOC_BUFFER				(BRB_ERROR - 100) 		/** Claim request violated buffer constraints */
#define BRB_ALLOC_INFO 					(BRB_ERROR - 101) 		/** Claim request violated buffer constraints */
#define BRB_ALLOC_STATS					(BRB_ERROR - 102)
#define BRB_ALLOC_BATCHES				(BRB_ERROR - 103) 		/** Claim request violated buffer constraints */
#define BRB_ALLOC_DATA 					(BRB_ERROR - 104) 		/** Claim request violated buffer constraints */

#define BRB_CLAIM_PANIC 				(BRB_ERROR - 200) 		/** Claim request violated buffer constraints */
#define BRB_CLAIM_FULL					(BRB_ERROR - 201)		
#define BRB_CLAIM_CANCELED				(BRB_ERROR - 202)		/** Claim request was canceled before it could be completed */

#define BRB_WRITE_BUFFER_FULL			(BRB_ERROR - 302)		/** Insufficient room in buffer to claim batch */

#define BRB_RELEASE_UNPUBLISHED			(BRB_ERROR - 400)		/** Requested batch was not yet published */
#define BRB_RELEASE_OVERFLOW			(BRB_ERROR - 401)		/** Requested batch release was out of order */

/* Batch Ring Buffer Methods */
extern int brb_init(brb_buffer ** buffer_ptr, uint64_t batch_buffer_size, uint64_t data_buffer_size, uint64_t data_size);
extern int brb_free(brb_buffer ** buffer_ptr);

/* Batch Methods */
extern int brb_reset(brb_batch * batch);
extern int brb_get(brb_buffer * buffer, brb_batch ** batch, uint64_t batch_num);
extern int brb_claim(brb_buffer * buffer, brb_batch ** batch, uint16_t count, void* cancel);
extern int brb_publish(brb_buffer * buffer, brb_batch * batch);
extern int brb_release(brb_buffer * buffer, brb_batch * batch);

#ifdef __cplusplus
}
#endif

#endif /* _BATCH_RING_BUFFER_H_ */
