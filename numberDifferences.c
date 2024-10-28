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
      // if number is bigger than 6, add 2 to range between numbers,
      // then add the difference between the number and the low number of the
      // range
      if (number > 6) {
        int lowerRange = abs(range[i - 1].lowNum - range[i - 1].highNum) + 2;
        int index = range[i].lowNum - number;
        rangeIndex = lowerRange + index + 1;
      } // if number is between 0 and 6,  set index to number + 1
      else if (number > 0 && number < 7) {
        rangeIndex = number + 1;
      } else {
        rangeIndex = number - range[i].lowNum;
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
