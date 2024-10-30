#include <stdint.h>
#include <stdio.h>

#include "bitStack.h"

void numberDifferences(bitStack *cValues, int number);

void absoluteEncoding(bitStack *cValues, int numberHolder);

void repeatingDifferences(bitStack *cValues, int repeatCount);

int compress(int *array, int arraySize, bitStack *cValues) {

  int firstNumber = array[0];
  for (int i = 0; i < 8; i++) {
    cValues->bits[cValues->index] =
        ((cValues->bits[cValues->index] << 1) | ((firstNumber >> i) & 1));
  }
  cValues->top = 8;
  int iterator = 1; // iterator for array of differences

  int numberHolder = 0;
  while (iterator < arraySize) {
    // take next number
    numberHolder = array[iterator];

    // if repeating numbers
    if (numberHolder == 0) {
      // found at least one
      int repeatCount = 0;
      // loop till it isnt the same number
      while (iterator + repeatCount < arraySize &&
             numberHolder == array[iterator + repeatCount]) {
        if (repeatCount == 8) {
          break;
        }
        repeatCount++;
      }
      iterator = iterator + repeatCount;
      repeatingDifferences(cValues, repeatCount);
      iterator--;
    }
    // if number needs 9 bits to write
    else if (numberHolder <= -30 || numberHolder >= 30) {
      absoluteEncoding(cValues, numberHolder);
    }
    // else write normal
    else if (numberHolder != 0) {
      numberDifferences(cValues, numberHolder);
    }
    iterator++;
  }
  ensureSpace(cValues);
  cValues->bits[cValues->index] |= (1 << cValues->top);
  cValues->top++;

  ensureSpace(cValues);
  cValues->bits[cValues->index] |= (1 << cValues->top);
  cValues->top++;

  printf("\n");
  return 0;
}
