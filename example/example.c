#include <src/ringbuffer.h>
//#include <src/batchringbuffer.h>
#include <stddef.h>
#include <stdio.h>

/*
void ring_example() {
	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;
	char * entry;

	rb_init(&buffer, 4, 1024);
	rb_claim(buffer, &seq_num, 1);
	rb_get(buffer, &entry, seq_num);
	rb_publish(buffer, seq_num, 1);
	rb_release(buffer, seq_num, 1);
	rb_free(&buffer);
}
*/
/*
void batch_example() {
	brb_buffer * buffer;
	brb_batch * batch;
	char cancel;

	printf("init -> claim -> publish -> release -> free\n");

	brb_init(&buffer, 4, 4, 1024);
	brb_claim(buffer, &batch, 1, &cancel);
	brb_publish(buffer, batch);
	brb_release(buffer, batch);
	brb_free(&buffer);
}
*/

int main() {
	/*batch_example();*/
	/*ring_example();*/

	return 0;
}
