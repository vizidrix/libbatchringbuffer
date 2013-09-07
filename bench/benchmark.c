#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>


#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#include <unistd.h>
#endif

#include "benchmark.h"

#define MILLIS 1E6
#define NANOS 1E9

typedef struct nano_clock {
	int64_t	sec;
	int64_t	nsec;
} nano_clock;

struct B {
	benchname_t					key;
	int							n;
	int 						running;
	void						* clock_token;
	struct nano_clock 			start_time;
	struct nano_clock			end_time;
	uint64_t					* samples;
	uint64_t					s_duration;
	uint64_t					ns_duration;
	b_bench_method		 		bench_method;
};

benchname_t
b_get_key(struct B * b) {
	return b->key;
}

int
b_count(struct B * b) {
	return b->n;
}

#ifdef __MACH__

inline uint64_t
get_nanos() {
	return mach_absolute_time();
}

void
get_timespec(nano_clock * nc) {
	nc->nsec = mach_absolute_time();
	nc->sec = nc->nsec / NANOS;
}

#else

inline uint64_t
get_nanos() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_nsec;
}

void
get_timespec(nano_clock * nc) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	nc->sec = ts.tv_sec;
	nc->nsec = ts.tv_nsec;
}

#endif

inline void
b_sample(struct B * b, int index) {
	b->samples[index] = get_nanos();
}

int
b_start_timer(struct B * b) {
	if(!b->running) {
		printf("starting");
		get_timespec(&b->start_time);
		b->samples[0] = b->start_time.nsec;
		b->running = 1;
	}

	return B_SUCCESS;
}

int
b_stop_timer(struct B * b) {
	uint64_t start, end, duration;
	if(b->running) {
		printf("stopping");
		get_timespec(&b->end_time);
		if (b->end_time.nsec - b->start_time.nsec < 0) {
			printf("Invalid nsec\n");
		}
		start = b->start_time.sec;
		end = b->end_time.sec;
		duration = end - start;
		b->s_duration = duration;

		start = b->start_time.nsec;
		end = b->end_time.nsec;
		duration = end - start;
		b->ns_duration = duration;

		b->running = 0;
	}

	return B_SUCCESS;
}

double
get_median(uint64_t data[], int size) {
	int i = 0;
	int j = 0;
	uint64_t temp = 0;
	uint64_t * sorted;
	double result;
	sorted = malloc(sizeof(uint64_t) * (size + 1));
	for (i = 1; i < size+1; i++) {
		sorted[i] = data[i] - data[i-1];
	}
	for (i = size - 1; i > 0; --i) {
		for(j = 0; j < i; ++j) {
			if(sorted[j] > sorted[j+1]) {
				temp = sorted[j];
				sorted[j] = sorted[j+1];
				sorted[j+1] = temp;
			}
		}
	}
	if((size & 1) == 0) {
		result = (double)(sorted[size / 2] + sorted[(size / 2) + 1]) / 2;
	} else {
		result = (double)sorted[(size / 2) + 1];
	}
	free(sorted);
	return result;
}

double
get_mean(uint64_t data[], int size) {
	int i = 0;
	uint64_t sum;
	sum = 0;
	for(i = 1; i < size+1; i++) {
		sum += data[i] - data[i-1];
	}
	return (double)(sum / size);
}

int
update_stats(struct B * b, struct BenchmarkResult * result) {
	result->ns_mean = get_mean(&b->samples[0], b->n);
	result->ns_median = get_median(&b->samples[0], b->n);

	result->s_mean = 0;
	result->s_median = 0;

	return B_SUCCESS;
}

int
b_exec_bench(struct BenchmarkResult * result, int count, benchname_t key, b_bench_method bench_method) {
	struct B b;
	double ms;
	double s;

	result->count = count;
	result->key = key;

	b.key = key;
	b.n = count;
	b.running = 0;
	b.samples = calloc(sizeof(uint64_t), count + 2);
	b.start_time.sec = 0;
	b.start_time.nsec = 0;
	b.end_time.sec = 0;
	b.end_time.nsec = 0;
	b.bench_method = bench_method;

	b.bench_method(&b);

	ms = (double)(b.ns_duration / MILLIS);
	s = (double)(b.ns_duration / NANOS);

	result->key = key;
	result->count = count;
	result->ns_per_op = (double)(b.ns_duration / count);
	result->ms_per_op = ms / count;
	result->s_per_op = s / count;
	result->ops_per_ms = (double)(count / ms);
	result->ops_per_s = (double)(count / s);
	result->s_duration = b.s_duration;
	result->ns_duration = b.ns_duration;

	update_stats(&b, result);

	free(b.samples);

	return B_SUCCESS;
}

#ifdef __MACH__

const char	* default_fmt = "\n%24s\t[%10d]\t[ %8.2f ns/op ]\t[ %'.2f op/s ]\n";
const char 	* stats_fmt = "\t\t\t\t%10s\t[ %8.2f ns/op ]\t[ %'.2f op/s ]\n";

#else

const char	* default_fmt = "\n%24s\t[%10d]\t[ %8.2f ns/op ]\t[ %8.2f op/s ]\n";
const char	* stats_fmt = "\t\t\t\t%10s\t[ %8.2f ns/op ]\t[ %8.2f op/s ]\n";

#endif

int
b_print_result(struct BenchmarkResult * result) {
	setlocale(LC_NUMERIC, "");
	printf(default_fmt,
		(char*)result->key,
		result->count,
		result->ns_per_op,
		result->ops_per_s);
	if(result->ns_median == 0) { return B_SUCCESS; }

	printf(stats_fmt,
		"MEAN:",
		result->ns_mean,
		(double)(NANOS / result->ns_mean));
	printf(stats_fmt,
		"MEDIAN:",
		result->ns_median,
		(double)(NANOS / result->ns_median)
		);

	return B_SUCCESS;
}


