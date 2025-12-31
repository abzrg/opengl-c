#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sds/sds.h"

// source: tsoding
#define DA_INIT_CAPACITY 256
#define DA_GROWTH_FACTOR 2
#define daAppend(da, item)                                                  \
  do {                                                                      \
    if ((da).count >= (da).capacity) {                                      \
      if ((da).capacity == 0)                                               \
        (da).capacity = DA_INIT_CAPACITY;                                   \
      else                                                                  \
        (da).capacity *= DA_GROWTH_FACTOR;                                  \
      (da).items = realloc((da).items, (da).capacity * sizeof((da).items)); \
    }                                                                       \
    (da).items[(da).count++] = (item);                                      \
  } while (0)

// typedef struct {
//   char* items;
//   size_t count;
//   size_t capacity;
// } String_Builder;

// String_Builder read_entire_file(const char* filename);

sds read_entire_file(const char* filename);

// vim: ft=c
