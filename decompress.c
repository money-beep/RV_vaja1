#include "bitRange.h"
#include "bitStack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int *decompress(bitStack *cValues, int arraySize) {
  BitRange range[] = {{-2, -1, 2},  {1, 2, 2},  {-6, -3, 3},   {3, 6, 3},
                      {-14, -7, 4}, {7, 14, 4}, {-30, -15, 5}, {15, 30, 5}};

  cValues->top = 0;
  cValues->index = 0;
  int dNumIterator = 0;
  int numberHolder = 0;
  int firstNumber = 0;

  for (int i = 0; i < 8; i++) {
    firstNumber =
        (firstNumber << 1) | ((cValues->bits[cValues->index] >> i) & 1);
    cValues->top++;
  }

  int *decompressedNumbers = (int *)malloc(sizeof(int) * arraySize);

  dNumIterator++;

  // creating array to hold number of differences
  int numberDifferences[arraySize];

  for (int i = 0; i < arraySize; i++) {
    numberDifferences[i] = 0;
  }

  while (1) {
    ensureReadSpace(cValues);
    numberHolder = 0;
    // if first bit is 0
    if (!((cValues->bits[cValues->index] >> cValues->top) & 1)) {
      cValues->top++;

      ensureReadSpace(cValues);
      // 00 - difference
      if (!((cValues->bits[cValues->index] >> cValues->top) & 1)) {
        cValues->top++;

        // printf("two read bits are 00, ");

        int differenceBit = 0;
        for (int i = 0; i < 2; i++) {
          ensureReadSpace(cValues);
          differenceBit = (differenceBit << 1) |
                          ((cValues->bits[cValues->index] >> cValues->top) & 1);
          cValues->top++;
        }
        differenceBit += 2;

        int rangeIndex = 0;
        for (int i = 0; i < differenceBit; i++) {
          ensureReadSpace(cValues);
          rangeIndex = (rangeIndex << 1) |
                       ((cValues->bits[cValues->index] >> cValues->top) & 1);
          cValues->top++;
        }

        // 8 ranges
        for (int i = 0; i < 8; i++) {
          if (differenceBit == range[i].bits) {
            if (range[i].highNum - range[i].lowNum < rangeIndex)
              numberHolder = rangeIndex - 1;
            else {
              numberHolder = range[i].lowNum + rangeIndex;
            }
            break;
          }
        }
        numberDifferences[dNumIterator] = numberHolder;
        dNumIterator++;

        printf("finished with difference, %d\n", numberHolder);

      }
      // 01 - repeating
      else if ((cValues->bits[cValues->index] >> cValues->top) & 1) {
        cValues->top++;

        // printf("two read bits are 01, ");

        for (int i = 0; i < 3; i++) {
          ensureReadSpace(cValues);
          numberHolder = (numberHolder << 1) |
                         ((cValues->bits[cValues->index] >> cValues->top) & 1);
          cValues->top++;
        }

        for (int i = dNumIterator; i < dNumIterator + numberHolder; i++) {
          if (i >= arraySize) {
            printf("Error: Index out of bounds!\n");
            break;
          }
          numberDifferences[i] = 0;
        }
        dNumIterator += numberHolder;

        printf("finished with repeating differences.\n");
      }
    } // first bit is 1
    else if ((cValues->bits[cValues->index] >> cValues->top) & 1) {
      cValues->top++;

      ensureReadSpace(cValues);
      // 10 - absolute
      if (!((cValues->bits[cValues->index] >> cValues->top) & 1)) {
        cValues->top++;

        // printf("two read bits are 10, ");

        ensureReadSpace(cValues);
        // first bit - sign, other 8 - abs(number)
        bool signBit = (cValues->bits[cValues->index] >> cValues->top) & 1;
        cValues->top++;

        for (int i = cValues->top++; i < cValues->top + 8; i++) {
          ensureReadSpace(cValues);
          numberHolder = (numberHolder << 1) |
                         ((cValues->bits[cValues->index] >> cValues->top) & 1);
          cValues->top++;
        }
        if (signBit)
          numberHolder = -numberHolder;
        numberDifferences[dNumIterator] = numberHolder;
        dNumIterator++;

        printf("finished with absolute encoding.\n");
      }
      // 11 - end
      if ((cValues->bits[cValues->index] >> cValues->top) & 1) {
        printf("two bits are 11, finished with decompression.\n");
        // konec
        break;
      }
    }
  }

  decompressedNumbers[0] = firstNumber;

  for (int i = 1; i < arraySize; i++) {
    decompressedNumbers[i] = decompressedNumbers[i - 1] + numberDifferences[i];
  }
  return decompressedNumbers;
}