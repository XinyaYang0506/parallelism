#ifndef MAP_H
#define MAP_H

#include <unistd.h>

typedef int (*map_fn_t)(int);

void map (map_fn_t fn, int* inputs, int* outputs, size_t len);

void parallel_map (map_fn_t fn, int* inputs, int* outputs, size_t len,
                   size_t threads);

#endif
