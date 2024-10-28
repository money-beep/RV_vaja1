#pragma once

typedef struct {
  long *bits; // can be used to create a dynamic stack of bits
  int top;    // used for tracking bit usage
  int index;  // used for tracking index of bits array
  int size;   // size tracking size
} bitStack;

void reallocStack(bitStack *cValues);

void ensureSpace(bitStack *cValues);

void ensureReadSpace(bitStack *cValues);
