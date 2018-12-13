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
  size_t len;
  int threads;
  size_t i;
} map_args;

void *
parallel_map_helper(void *arg)
{
  map_args *args = (map_args *)arg;
  size_t i = args->i;
  size_t len = args->len;
  int threads = args->threads;
  int assignment = len / threads;
  int real_len = len / threads;
  int *inputs = args->inputs;
  int *outputs = args->outputs;
  map_fn_t fn = args->fn;
  if (i == args->threads - 1)
  {
    real_len = len - (real_len * i);
  }

  size_t start = i * assignment;
  size_t end = i * assignment + real_len;
  for (size_t j = start; j < end; j++)
  {
    outputs[j] = fn(inputs[j]);
  }
  
  return NULL;
}

/*
 * Apply fn to every entry in the inputs array, writing the result in the
 * outputs array Run in parallel using a fixed number of threads.
 */
void parallel_map(map_fn_t fn, int *inputs, int *outputs, size_t len,
                  size_t threads)
{
  // Create threads
  pthread_t threads_arr[threads];
  // Create a array of arguments
  map_args arg_arr[threads];
  for (size_t i = 0; i < threads; i++)
  {
    // map_args args = arg_arr[i];
    arg_arr[i].inputs = inputs;
    arg_arr[i].outputs = outputs;
    arg_arr[i].len = len;
    arg_arr[i].fn = fn;
    arg_arr[i].threads = threads;
    arg_arr[i].i = i;
    thread_create(&threads_arr[i], parallel_map_helper, (void *)&arg_arr[i]);
  }
  
  for (size_t i = 0; i < threads; i++)
  {
    thread_join(threads_arr[i]);
  }
}
