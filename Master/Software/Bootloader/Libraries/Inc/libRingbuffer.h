#ifndef RingBuf_h
#define RingBuf_h

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//https://github.com/wizard97/ArduinoRingBuffer

typedef struct libRingBufferTypedef libRingBufferTypedef;

typedef struct libRingBufferTypedef {
  // Invariant: end and start is always in bounds
  unsigned char *buf;
  unsigned int len, size, start, end, elements;

  // Private:
  int (*next_end_index) (libRingBufferTypedef*);
  int (*incr_end_index) (libRingBufferTypedef*);

  int (*incr_start_index) (libRingBufferTypedef*);

  //public:
  // Returns true if full
  bool (*isFull) (libRingBufferTypedef*);
  // Returns true if empty
  bool (*isEmpty) (libRingBufferTypedef*);
  // Returns number of elemnts in buffer
  unsigned int (*numElements)(libRingBufferTypedef*);
  // Add Event, Returns index where added in buffer, -1 on full buffer
  int (*add) (libRingBufferTypedef*, const void*);
  // Returns pointer to nth element, NULL when nth element is empty
  void *(*peek) (libRingBufferTypedef*, unsigned int);
  // Removes element and copies it to location pointed to by void *
  // Returns pointer passed in, NULL on empty buffer
  void *(*pull) (libRingBufferTypedef*, void *);

} libRingBufferTypedef;

libRingBufferTypedef *libRingBufferNew(int size, int len);
int libRingBufferInit(libRingBufferTypedef *self, int size, int len);
int libRingBufferDelete(libRingBufferTypedef *self);
int libRingBufferNextEndIndex(libRingBufferTypedef *self);
int libRingBufferIncrEnd(libRingBufferTypedef *self);
int libRingBufferIncrStart(libRingBufferTypedef *self);
int libRingBufferAdd(libRingBufferTypedef *self, const void *object);
void *libRingBufferPeek(libRingBufferTypedef *self, unsigned int num);
void *libRingBufferPull(libRingBufferTypedef *self, void *object);
bool libRingBufferIsFull(libRingBufferTypedef *self);
bool libRingBufferIsEmpty(libRingBufferTypedef *self);
unsigned int libRingBufferNumElements(libRingBufferTypedef *self);

#endif
