#include "libRingbuffer.h"

libRingBufferTypedef *libRingBufferNew(int size, int len) {
  libRingBufferTypedef *self = (libRingBufferTypedef *)malloc(sizeof(libRingBufferTypedef));
	
  if (!self)
		return NULL;
	
  memset(self, 0, sizeof(libRingBufferTypedef));
	
  if (libRingBufferInit(self, size, len) < 0) {
    free(self);
    return NULL;
  }
  return self;
};

int libRingBufferInit(libRingBufferTypedef *self, int size, int len) {
  self->buf = (unsigned char *)malloc(size*len);
  if (!self->buf) return -1;
  memset(self->buf, 0, size*len);

  self->size = size;
  self->len = len;
  self->start = 0;
  self->end = 0;
  self->elements = 0;

  self->next_end_index = &libRingBufferNextEndIndex;
  self->incr_end_index = &libRingBufferIncrEnd;
  self->incr_start_index = &libRingBufferIncrStart;
  self->isFull = &libRingBufferIsFull;
  self->isEmpty = &libRingBufferIsEmpty;
  self->add = &libRingBufferAdd;
  self->numElements = &libRingBufferNumElements;
  self->peek = &libRingBufferPeek;
  self->pull = &libRingBufferPull;
  return 0;
};

int libRingBufferDelete(libRingBufferTypedef *self) {
  free(self->buf);
  free(self);
  return 0;
};

/////// PRIVATE METHODS //////////
// get next empty index
int libRingBufferNextEndIndex(libRingBufferTypedef *self) {
  //buffer is full
  if (self->isFull(self)) return -1;
  //if empty dont incriment
  return (self->end+(unsigned int)!self->isEmpty(self))%self->len;
};

// incriment index of RingBuf struct, only call if safe to do so
int libRingBufferIncrEnd(libRingBufferTypedef *self) {
  self->end = (self->end+1)%self->len;
  return self->end;
};


// incriment index of RingBuf struct, only call if safe to do so
int libRingBufferIncrStart(libRingBufferTypedef *self) {
  self->start = (self->start+1)%self->len;
  return self->start;
};

/////// PUBLIC METHODS //////////
// Add an object struct to RingBuf
int libRingBufferAdd(libRingBufferTypedef *self, const void *object) {
  int index;
  // Perform all atomic opertaions
	index = self->next_end_index(self);
	//if not full
	if (index >= 0) {
		memcpy(self->buf + index*self->size, object, self->size);
		if (!self->isEmpty(self)) self->incr_end_index(self);
		self->elements++;
	}

  return index;
}

// Return pointer to num element, return null on empty or num out of bounds
void *libRingBufferPeek(libRingBufferTypedef *self, unsigned int num) {
  void *ret = NULL;

  //empty or out of bounds
	if (self->isEmpty(self) || num > self->elements - 1)
		ret = NULL;
	else
		ret = &self->buf[((self->start + num)%self->len)*self->size];

  return ret;
}

// Returns and removes first buffer element
void *libRingBufferPull(libRingBufferTypedef *self, void *object) {
  void *ret = NULL;

	if (self->isEmpty(self))
		ret = NULL;
	else{
		memcpy(object, self->buf+self->start*self->size, self->size);
		self->elements--;
		// don't incriment start if removing last element
		if (!self->isEmpty(self)) self->incr_start_index(self);
		ret = object;
	}
  
  return ret;
}

// Returns number of elemnts in buffer
unsigned int libRingBufferNumElements(libRingBufferTypedef *self) {
  unsigned int elements;

  elements = self->elements;
	
  return elements;
}

// Returns true if buffer is full
bool libRingBufferIsFull(libRingBufferTypedef *self) {
  bool ret;

  ret = self->elements == self->len;  

  return ret;
}

// Returns true if buffer is empty
bool libRingBufferIsEmpty(libRingBufferTypedef *self) {
  bool ret;

  ret = !self->elements;

  return ret;
}
