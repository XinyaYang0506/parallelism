#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "map.h"

#define NUM_INPUTS 16384

int largest_prime_factor (int n);
bool is_prime (int n);
size_t time_ms ();

int main () {
  int inputs[NUM_INPUTS];
  int serial_outputs[NUM_INPUTS];
  int parallel_outputs[NUM_INPUTS];
	
  // Seed the random number generator
  srand(42);
	
  // Initialize with "random" inputs
  for (size_t i=0; i<NUM_INPUTS; i++) {
    inputs[i] = rand() % 100000;
  }
	
  // Zero out the outputs arrays
  memset(serial_outputs, 0, sizeof(int) * NUM_INPUTS);
  memset(parallel_outputs, 0, sizeof(int) * NUM_INPUTS);
	
  // Run the map in serial
  size_t serial_time = time_ms();
  map(largest_prime_factor, inputs, serial_outputs, NUM_INPUTS);
  serial_time = time_ms() - serial_time;
	
  printf("Runtime in serial: %lums\n", serial_time);
	
  // Run the map in parallel
  for (size_t threads = 1; threads <= 8; threads *= 2) {
    size_t parallel_time = time_ms();
    parallel_map(largest_prime_factor, inputs, parallel_outputs, NUM_INPUTS,
                 threads);
    parallel_time = time_ms() - parallel_time;
	
    // Validate the outputs
    for (size_t i=0; i<NUM_INPUTS; i++) {
      if (parallel_outputs[i] != serial_outputs[i]) {
        printf("Output from parallel_map with %lu thread%s does not match!\n",
               threads, threads == 1 ? "" : "s");
        break;
      }
    }
		
    printf("Runtime with %lu thread%s: %lums\n", threads, threads == 1 ? "" : "s",
           parallel_time);
  }
	
  return 0;
}

/*
 * Find the largest prime factor of n
 */
int largest_prime_factor (int n) {
  int largest_factor = 1;
  for (int d=2; d<=n; d++) {
    if (n % d == 0 && is_prime(d)) {
      largest_factor = d;
    }
  }
  return largest_factor;
}

/*
 * Check if the given value is prime
 */
bool is_prime (int n) {
  for (int d=2; d*d<=n; d++) {
    if (n % d == 0) {
      return false;
    }
  }
  return true;
}

/*
 * Get the time in milliseconds since UNIX epoch
 */
size_t time_ms() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == -1) {
    perror("gettimeofday");
    exit(2);
  }
  
  // Convert timeval values to milliseconds
  return tv.tv_sec*1000 + tv.tv_usec/1000;
}
