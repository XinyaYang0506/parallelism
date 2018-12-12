#include "map.h"

#include "../mythreads.h"
#include <unistd.h>

/*
 * Apply fn to every entry in the inputs array, writing the result in the
 * outputs array
 */
void map(map_fn_t fn, int* inputs, int* outputs, size_t len) {
  for(size_t i=0; i<len; i++) {
    outputs[i] = fn(inputs[i]);
  }
}

/*
 * Apply fn to every entry in the inputs array, writing the result in the
 * outputs array Run in parallel using a fixed number of threads.
 */
void parallel_map(map_fn_t fn, int* inputs, int* outputs, size_t len,
                  size_t threads) {
  // TODO: Implement me!
}

