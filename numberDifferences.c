#include "bitRange.h"
#include "bitStack.h"
#include <stdlib.h>

void neededBits(bitStack *cValues, int number) {

  ensureSpace(cValues);

  switch (number) {
  case 0:
    cValues->top++;

    ensureSpace(cValues);
    cValues->top++;
    break;
  case 1:
    cValues->top++;

    ensureSpace(cValues);
    cValues->bits[cValues->index] |= (1 << cValues->top);
    cValues->top++;
    break;
  case 2:
    cValues->bits[cValues->index] |= (1 << cValues->top);
    cValues->top++;

    ensureSpace(cValues);
    cValues->top++;
    break;
  case 3:
    cValues->bits[cValues->index] |= (1 << cValues->top);
    cValues->top++;

    ensureSpace(cValues);
    cValues->bits[cValues->index] |= (1 << cValues->top);
    cValues->top++;
    break;
  }
}

void numberDifferences(bitStack *cValues, int number) {
  BitRange range[] = {{-2, -1, 2},  {1, 2, 2},  {-6, -3, 3},   {3, 6, 3},
                      {-14, -7, 4}, {7, 14, 4}, {-30, -15, 5}, {15, 30, 5}};

  ensureSpace(cValues);
  cValues->top++;

  ensureSpace(cValues);
  cValues->top++;

  for (int i = 0; i < (int)(sizeof(range) / sizeof(range[0])); i++) {
    if (number >= range[i].lowNum && number <= range[i].highNum) {
      int rangeIndex = 0;

      if (number < 0) {
        rangeIndex = abs(range[i].lowNum) - abs(number);
      } else {
        rangeIndex = number - range[i].lowNum;
        rangeIndex |= 1 << (range[i].bits - 1);
      }

      // 00 - 2, 01 - 3, 10 - 4, 11 - 5
      int differenceBit = range[i].bits - 2;
      neededBits(cValues, differenceBit);

      // printf("index: %d, now in bits: ", rangeIndex);
      for (int j = range[i].bits - 1; j >= 0; j--) {
        // printf("%d", (rangeIndex >> j) & 1);
        ensureSpace(cValues);

        // if bit is 1
        if ((rangeIndex >> j) & 1) {
          // set the value at top index to 1
          cValues->bits[cValues->index] |= (1 << cValues->top);
        }
        cValues->top++;
      }
    }
  }
}
