#include <src/batchringbuffer.h>
#include <stddef.h>
#include <stdio.h>

/*int main(int argc, char** argv)*/
int main()
{
	brb_buffer * buffer;
	brb_batch * batch;
	char cancel;

	brb_init_buffer(&buffer, 4, 4, 1024);
	brb_claim(buffer, &batch, 1, &cancel);
	brb_publish(buffer, batch);
	brb_release(buffer, batch);
	brb_free_buffer(buffer);

	printf("init -> claim -> publish -> release -> free\n");
	return 0;
}
