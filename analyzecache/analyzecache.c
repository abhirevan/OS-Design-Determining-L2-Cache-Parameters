#include "time.h"
#include "analyzecache.h"
#include "stdlib.h"
#include "stdio.h"

#define TOTAL_MEM_SIZE (4096 * 1024*4) // Memory size
#define L1_MAX_CACHE_MULTIPLIER 32768*8 // Max cache size multiplier considered considered
#define L1_MAX_CACHE_LINE_MULTIPLIER 128 // Max Cache line size considered

float analyze_cache_line_size(int* a, int cache_line_multiplier) {
	int i, j;
	volatile int sum = 0;
	//int sum=0;
	int num_of_elements = TOTAL_MEM_SIZE / sizeof(int);

	struct timeval t1, t2;
	gettimeofday(&t1, 0);

	for (i = 0, j = 0; j < num_of_elements;
			j++, i = (i + cache_line_multiplier / sizeof(int)) % num_of_elements) {
		sum += a[i];
	}
	gettimeofday(&t2, 0);

	return ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));
}

float analyze_cache_size(int* a, int cache_multiplier, int cache_line_size) {
	int i, j;
	volatile int sum = 0;
	//int sum = 0;
	int iter = TOTAL_MEM_SIZE / (cache_line_size * sizeof(int));

	struct timeval t1, t2;
	gettimeofday(&t1, 0);

	for (i = 0, j = 0; j < iter;
			j++, i = (i + (cache_multiplier * cache_line_size) / sizeof(int))
					% iter) {
		sum += a[i];
	}
	gettimeofday(&t2, 0);

	return ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));
}

float calculate_cache_miss_penalty(int* a, int cache_size) {
	int i, j;
	volatile int sum = 0;
	//int sum = 0;
	struct timeval t1, t2;
	gettimeofday(&t1, 0);
	int num_of_iterations = TOTAL_MEM_SIZE / (cache_size * sizeof(int) * 2);
	for (i = 0, j = 0; i < num_of_iterations;
			i++, j = j + 2 * cache_size / sizeof(int)) {
		sum += a[j];
	}
	gettimeofday(&t2, 0);

	return ((t2.tv_sec - t1.tv_sec) * 1000000000
			+ (t2.tv_usec - t1.tv_usec) * 1000) / num_of_iterations;
}

void resetArray(int* a) {
	int k;
	for (k = 0; k < TOTAL_MEM_SIZE / sizeof(int); k++) {
		a[k] = 1;
	}
}

int main(int argc, char *argv[]) {
	register int *a, *b, i, j, k;

	float prev, current;
	int cache_line_size;
	float max_spike = -1, diff;
	int max_spike_cache_size = 0;

	b = a = (int*) malloc(TOTAL_MEM_SIZE);
	resetArray(a);
	//For cache line size
	printf("For Cache Line Size:\n");
	printf("CacheLineSize(B)  Time\n");
	for (i = 4; i <= L1_MAX_CACHE_LINE_MULTIPLIER; i <<= 1) {
		a = b;
		current = analyze_cache_line_size(a, i);
		if (i == 4) {
			prev = current;
		}
		printf("%10d : %10.3f\n", i, current);
		diff = abs(current - prev);
		if (max_spike < diff) {
			max_spike = diff;
			max_spike_cache_size = i;
		}
		prev = current;

	}
	free(a);
	cache_line_size = max_spike_cache_size / 2;

	int cache_size;
	max_spike = -1;
	max_spike_cache_size = 0;

	b = a = (int*) malloc(TOTAL_MEM_SIZE);
	resetArray(a);

	//For cache size
	printf("For Cache Size:\n");
	printf("CacheMultiplier CacheSize(B)  Time\n");
	for (i = 1; i <= L1_MAX_CACHE_MULTIPLIER; i <<= 1) {
		a = b;
		current = analyze_cache_size(a, i, cache_line_size);
		if (i == 1) {
			prev = current;
		}

		printf("%10d : %10d : %10.3f\n", i, i * cache_line_size, current);

		diff = abs(current - prev);
		//printf("Diff: %f\n",diff);
		if (max_spike < diff) {
			max_spike = diff;
			max_spike_cache_size = i;
		}
		prev = current;
	}
	free(a);

	//printf("Max spike: %f\n",max_spike);

	cache_size = max_spike_cache_size / 2;

	b = a = (int*) malloc(TOTAL_MEM_SIZE);
	resetArray(a);
	free(a);

	//Print final report
	printf("\nFinal Report\n");
	printf("Cache Block/Line Size: %d B\n", cache_line_size);
	printf("Cache Size: %d KB\n", cache_size * cache_line_size / 1024);
	printf("Cache Miss Penalty: %f us\n",
			calculate_cache_miss_penalty(a, cache_size) / 1000);

	return 0;
}
