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

#ifndef __x86_64__
#warning "The program is developed for x86-64 architecture only."
#endif
#if !defined(DCACHE1_LINESIZE) || !DCACHE1_LINESIZE
#ifdef DCACHE1_LINESIZE
#undef DCACHE1_LINESIZE
#endif
#define ____cacheline_aligned	__attribute__((aligned(64)))
#else
#define ____cacheline_aligned	__attribute__((aligned(DCACHE1_LINESIZE)))
#endif

typedef enum {
	AVAILABLE = 1, 
	WRITING = 2,
	CANCELED = 3,
	PUBLISHED = 4
} brb_batch_states;

typedef struct brb_buffer_info {
	uint64_t		batch_buffer_size;	/** < Number of slots allocated for use in tracking batches */
	uint64_t		batch_size_mask;	/** < Batch buffer size - 1; Used to keep batches inside the ring */
	uint64_t		data_buffer_size;	/** < Number of slots allocated per the rules above */
	uint64_t		data_size_mask;		/** < Data buffer size - 1; Used to keep data inside the ring */
	uint64_t		entry_size;			/** < Number of bytes of data for each slot */
	uint64_t		total_data_size;	/** < Total number of bytes allocated to the buffer */
} brb_buffer_info;

typedef struct brb_buffer_stats {
	volatile uint64_t	barrier_batch_num 	____cacheline_aligned;	/** < Index of the oldest batch that is still in use by at least one reader */
	volatile uint64_t	read_batch_num 		____cacheline_aligned;	/** < Index of the newest batch which has been released to readers */
	volatile uint64_t	write_batch_num 	____cacheline_aligned;	/** < Index of the next available batch to be assigned to a writer */
	volatile uint64_t	barrier_seq_num 	____cacheline_aligned;	/** < Index of the oldest seq num that is still held by at least one reader (don't overflow) */
	volatile uint64_t	write_seq_num 		____cacheline_aligned;	/** < Index of the next available entry for allocation to a batch */
} brb_buffer_stats;

typedef struct brb_process_result {
	uint64_t	releases;
	uint64_t	claims;
	uint64_t	publishes;
} brb_process_result;

typedef struct brb_batch {
	/** <  The counter used to & to find the index of this batch in the buffer */
	volatile uint64_t			batch_num;
	/** <  How many entries to claim for this batch */
	volatile uint64_t 			batch_size;
	/** <  Seq number assigend to this batch */
	volatile uint64_t 			seq_num;
	/** <  Padding to keep other data elements out of this cache line */
	volatile uint64_t			data[5];					
	/** <  Current state of the batch */
	volatile brb_batch_states 	state 				____cacheline_aligned;
	/** < Readers are assigned a group bit mask to & once all of their group are complete */
	volatile uint64_t 			group_flags			____cacheline_aligned;
	/** < Readers are assigned a bit mask to & once complete */
	volatile uint64_t 			reader_flags[8]		____cacheline_aligned;
} brb_batch;

/* May be used to hold reader subscription info
typedef struct brb_reader {

} brb_reader;
*/

typedef struct brb_buffer brb_buffer;

#define BRB_SUCCESS 					0 						/** Successful result */
#define BRB_ERROR						(-40600)				/** Generic error */

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

extern int brb_reset_batch(brb_batch * batch);

extern int brb_init_buffer(brb_buffer ** buffer_ptr, uint64_t batch_buffer_size, uint64_t data_buffer_size, uint64_t data_size);
extern int brb_free_buffer(brb_buffer * buffer);

extern int brb_get_batch(brb_buffer * buffer, brb_batch ** batch, uint64_t batch_num);
extern int brb_get_entry(brb_buffer * buffer, void ** entry, uint64_t seq_num);

/* This will block until the batch is claimed */
extern int brb_claim(brb_buffer * buffer, brb_batch ** batch, uint16_t count, void* cancel);
/*
This is basically a trigger to clear the batch for processing by readers
	- Readers will (should) process batches in claimed sequence
	- Can watch state at this batch slot to tell when batch is finished writing
	- Can put state info into batch data for readers
*/
extern int brb_publish(brb_buffer * buffer, brb_batch * batch);
/*
This is a trigger to update the batch's state flag
*/
extern int brb_release(brb_buffer * buffer, brb_batch * batch);
/*
Registers a reader with the buffer
	- Group num translates into group mask bit position
	- Reader mask is assigned by the buffer
*/
/*
extern brb_reader * brb_subscribe(brb_buffer * buffer, uint8_t group_num);
*/
/*
Frees the memory for the reader
*/
/*
extern void brb_unsubscribe(brb_reader * reader);
*/

extern int brb_get_info(brb_buffer * buffer, brb_buffer_info ** info);
extern int brb_get_stats(brb_buffer * buffer, brb_buffer_stats ** info);


#ifdef __cplusplus
}
#endif


#endif /* _BATCH_RING_BUFFER_H_ */
