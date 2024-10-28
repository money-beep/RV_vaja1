#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitStack.h"

void compress(int *array, int arraySize, bitStack *cValues);

int *decompress(bitStack *cValues, int arraySize);

// generates number based on the user picked maximum difference in random
// number and returns an array which holds the difference in numbers: a[i] -
// a[i+1];
int *initArray(int answerDiff, int answerCount) {
  int minNum;
  int *diffArray = (int *)malloc(answerCount * sizeof(int));
  int *numArray = (int *)malloc(answerCount * sizeof(int));

  numArray[0] = rand() % 256;

  for (int i = 1; i < answerCount; i++) {
    minNum = numArray[i - 1] - (answerDiff);
    numArray[i] = minNum + rand() % answerDiff * 2;
  }

  for (int i = 0; i < answerCount; i++) {
    printf("%d ", numArray[i]);
  }
  printf("\n");

  diffArray[0] = numArray[0];

  for (int i = 1; i < answerCount; i++) {
    diffArray[i] = numArray[i] - numArray[i - 1];
  }

  return diffArray;
}

double compressionRatio(bitStack *cValues, int numElements) {
  return (double)(cValues->index * 32 + cValues->top) / (numElements * 8);
}

int main() {
  srand(time(0));

  int countNum[] = {5, 50, 500, 5000};
  int differenceNum[] = {5, 10, 15, 30};

  int answerCount = -1;
  int answerDiff = -1;

  while ((answerCount < 0 || answerCount > 3) &&
         (answerDiff < 0 || answerDiff > 3)) {
    printf("Choose number count (0 - 5, 1 - 50, 2 - 500, 3 - 5000): ");
    scanf("%d", &answerCount);

    printf("Choose max difference between numbers (0 - 5, 1 - 10, 2 - 15, 3 - "
           "30): ");
    scanf("%d", &answerDiff);
  }

  answerCount = countNum[answerCount];
  answerDiff = differenceNum[answerDiff];

  int *diffArray = initArray(answerDiff, answerCount);

  bitStack *cValues = (bitStack *)malloc(sizeof(bitStack));
  cValues->size = 2;
  cValues->bits = (long *)malloc(sizeof(long) * cValues->size);
  cValues->index = 0;
  cValues->top = 0;

  compress(diffArray, answerCount, cValues);

  int *decompressedNumbers;
  decompressedNumbers = decompress(cValues, answerCount);

  for (int i = 0; i < answerCount; i++) {
    printf("%d ", decompressedNumbers[i]);
  }
  printf("\n\nDifferences: ");

  for (int i = 0; i < answerCount; i++)
    printf("%d ", diffArray[i]);
  printf("\n");
  for (int j = 0; j <= cValues->index; j++) {
    for (int i = 0; i <= 31; i++)
      printf("%ld", (cValues->bits[j] >> i) & 1);
    printf("\n");
  }

  printf("compression ratio is: %f\n", compressionRatio(cValues, answerCount));
}
