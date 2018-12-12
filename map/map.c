#include "map.h"

#include "../mythreads.h"
#include <unistd.h>

/*
 * Apply fn to every entry in the inputs array, writing the result in the
 * outputs array
 */
void map(map_fn_t fn, int *inputs, int *outputs, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    outputs[i] = fn(inputs[i]);
  }
}

typedef struct
{
  map_fn_t fn;
  int *inputs;
  int *outputs;
  int len;
} map_args;

/*
 * Apply fn to every entry in the inputs array, writing the result in the
 * outputs array Run in parallel using a fixed number of threads.
 */
void parallel_map(map_fn_t fn, int *inputs, int *outputs, size_t len,
                  size_t threads)
{
  // Create threads
  pthread_t threads[threads];
  int real_len = len / threads;
  for (size_t i = 0; i < threads; i++)
  {
    map_args args;
    args.inputs = *inputs + (i * real_len);
    args.outputs = *outputs + (i * real_len);
    args.len = real_len;
    args.fn = fn;

    thread_create(&threads[i], map, (void *)&args));
  }

  for (size_t i = 0; i < threads; i++)
  {
    thread_join(threads[i]);
  }
}
