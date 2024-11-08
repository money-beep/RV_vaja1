#include "bitStack.h"
#include <stdlib.h>

void absoluteEncoding(bitStack *cValues, int number) {
  ensureSpace(cValues);
  cValues->bits[cValues->index] |= (1 << cValues->top);
  cValues->top++;

  ensureSpace(cValues);
  cValues->top++;

  ensureSpace(cValues);
  if (number < 0) {
    cValues->bits[cValues->index] |= (1 << cValues->top);
    number = abs(number);
  }
  cValues->top++;

  for (int i = 7; i >= 0; i--) {
    ensureSpace(cValues);
    if ((number >> i) & 1) {
      cValues->bits[cValues->index] |= (1 << cValues->top);
    }
    cValues->top++;
  }
}
