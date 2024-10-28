#include "bitStack.h"

// needs to input number of repeating numbers in given array from start index
void repeatingDifferences(bitStack *cValues, int repeatCount) {

  if (repeatCount < 0)
    repeatCount--;

  ensureSpace(cValues);
  cValues->top++;

  ensureSpace(cValues);
  cValues->bits[cValues->index] |= (1 << cValues->top);
  cValues->top++;

  // 3 bits
  for (int i = 2; i >= 0; i--) {
    ensureSpace(cValues);
    if ((repeatCount >> i) & 1) {
      // set the value at top index to 1
      cValues->bits[cValues->index] |= (1 << cValues->top);
    }
    cValues->top++;
  }
}
