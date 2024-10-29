#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitStack.h"

void compress(int *array, int arraySize, bitStack *cValues);

int *decompress(bitStack *cValues, int arraySize);

double compressionRatio(bitStack *cValues, int numElements) {
  return (double)(cValues->index * 32 + cValues->top) / (numElements * 8);
}

void outputToFile(int *numArray, int *diffArray, int *decompressedNumbers,
                  bitStack *cValues, int arraySize) {
  // output to file first numbers, differences, compressed bits, decompressed
  // numbers
  FILE *f = fopen("output.txt", "w");

  fprintf(f, "Numbers:\n");
  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", numArray[i]);
  }

  fprintf(f, "\n\n Differences:\n");

  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", diffArray[i]);
  }

  fprintf(f, "\n\n Compressed bits:\n");

  for (int j = 0; j <= cValues->index; j++) {
    for (int i = 0; i <= 31; i++) {
      fprintf(f, "%ld", (cValues->bits[j] >> i) & 1);
    }
    fprintf(f, "\n");
  }

  fprintf(f, "\n\n Decompressed numbers:\n");

  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", decompressedNumbers[i]);
  }

  fclose(f);
}

int main() {
  srand(time(0));

  int countNum[] = {10, 100, 500, 1000};
  int differenceNum[] = {5, 15, 30, 50};

  int answerCount = -1;
  int answerDiff = -1;

  while ((answerCount < 0 || answerCount > 3) &&
         (answerDiff < 0 || answerDiff > 3)) {
    printf("Choose number count (0 - 10, 1 - 100, 2 - 500, 3 - 1000): ");
    scanf("%d", &answerCount);

    printf("Choose max difference between numbers (0 - 5, 1 - 15, 2 - 30, 3 - "
           "50): ");
    scanf("%d", &answerDiff);
  }

  answerCount = countNum[answerCount];
  answerDiff = differenceNum[answerDiff];

  int *numArray;
  int *diffArray;
  diffArray = (int *)malloc(answerCount * sizeof(int));
  numArray = (int *)malloc(answerCount * sizeof(int));

  // generates number based on the user picked maximum difference in random
  // number and returns an array which holds the difference in numbers: a[i] -
  // a[i+1];
  int minNum;
  numArray[0] = rand() % 256;
  if (numArray[0] < 100) {
    numArray[0] += 50;
  }

  for (int i = 1; i < answerCount; i++) {
    minNum = numArray[i - 1] - (answerDiff / 2);
    numArray[i] = minNum + rand() % answerDiff;

    if (numArray[i] < 0) {
      numArray[i] = abs(numArray[i]);
    }
  }

  diffArray[0] = numArray[0];

  for (int i = 1; i < answerCount; i++) {
    diffArray[i] = numArray[i] - numArray[i - 1];
  }

  printf("Original numbers:\n");
  for (int i = 0; i < answerCount; i++) {
    printf("%d ", numArray[i]);
  }
  printf("\nDifferences:\n");
  for (int i = 0; i < answerCount; i++) {
    printf("%d ", diffArray[i]);
  }
  printf("\n");

  bitStack *cValues = (bitStack *)malloc(sizeof(bitStack));
  cValues->size = 2;
  cValues->bits = (long *)malloc(sizeof(long) * cValues->size);
  cValues->index = 0;
  cValues->top = 0;

  printf("Compressing...\n");
  compress(diffArray, answerCount, cValues);

  printf("Decompressing...\n");
  int *decompressedNumbers;
  decompressedNumbers = decompress(cValues, answerCount);

  printf("Decompressed numbers:\n");
  for (int i = 0; i < answerCount; i++) {
    printf("%d ", decompressedNumbers[i]);
  }

  printf("Compression ratio is: %f\n", compressionRatio(cValues, answerCount));

  outputToFile(numArray, diffArray, decompressedNumbers, cValues, answerCount);
}
