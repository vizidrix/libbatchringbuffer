#include <src/batchringbuffer.h>

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
/*#include <iostream>*/
/*using namespace std;*/
#endif

#define MILLIS 1000000L
#define NANOS 1000000000L

struct timespec
diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void
get_timespec(struct timespec * ts) {
#ifdef __MACH__ /* OS X does not have clock_gettime, use clock_get_time */
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;

#else
	clock_gettime(CLOCK_REALTIME, ts);
#endif
}

double
bench(int n)
{
	struct timespec time1, time2;
	
	brb_buffer * buffer;
	brb_batch * batch;
	char cancel;
	int i;

	brb_init_buffer(&buffer, 4, 4, 1024);
	get_timespec(&time1);
	for (i = 0; i< n; i++) {
		
		brb_claim(buffer, &batch, 1, &cancel);
		brb_publish(buffer, batch);
		brb_release(buffer, batch);
		
	}
	get_timespec(&time2);
	brb_free_buffer(&buffer);
	return (double)diff(time1,time2).tv_nsec / (double)n;
}

int main() {
	double ns_per_op;
	double ops_per_ms;
	double ops_per_s;
	int count;
	printf("\nbench.c - main()\n\n\n");
	/* warmup */
	bench(1000);
	for(count = 1000; count <= 1000000; count *= 10) {
		ns_per_op = bench(count);
		ops_per_ms = MILLIS / ns_per_op;
		ops_per_s = NANOS / ns_per_op;
		printf("[%10d]\t%8.4f ns/op\t\t%12.4f op/ms\t\t%12.4f op/s\n", count, ns_per_op, ops_per_ms, ops_per_s);
	}
	printf("\n\n/ bench.c - main()\n\n");
	return 0;
}
