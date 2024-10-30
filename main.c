#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitStack.h"

void compress(int *array, int arraySize, bitStack *cValues);

int *decompress(bitStack *cValues, int arraySize);

double compressionRatio(bitStack *cValues, int numElements) {
  return (double)((cValues->index - 1) * 32 + cValues->top) /
         (numElements * 32);
}

void outputToFile(int *numArray, int *diffArray, int *decompressedNumbers,
                  bitStack *cValues, int arraySize, double compTime,
                  double DecompTime) {

  // output to file first numbers, differences, compressed bits, decompressed
  // numbers
  FILE *f = fopen("output.txt", "w");

  fprintf(f, "Numbers:\n");
  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", numArray[i]);
  }

  fprintf(f, "\n\nDifferences:\n");

  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", diffArray[i]);
  }

  fprintf(f, "\n\nCompressed bits:\n");

  for (int j = 0; j <= cValues->index; j++) {
    for (int i = 0; i <= 31; i++) {
      fprintf(f, "%ld", (cValues->bits[j] >> i) & 1);
    }
    fprintf(f, "\n");
  }

  fprintf(f, "\n\nDecompressed numbers:\n");

  for (int i = 0; i < arraySize; i++) {
    fprintf(f, "%d ", decompressedNumbers[i]);
  }

  fprintf(f, "\n\nCompression ratio: %f\n",
          compressionRatio(cValues, arraySize));

  fprintf(f, "Compression time: %f\n", compTime);
  fprintf(f, "Decompression time: %f\n", DecompTime);

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
    minNum = numArray[i - 1] - (answerDiff);
    numArray[i] = minNum + rand() % answerDiff;

    if (numArray[i] < 0) {
      numArray[i] = abs(numArray[i]);
    }
  }

  diffArray[0] = numArray[0];

  for (int i = 1; i < answerCount; i++) {
    diffArray[i] = numArray[i] - numArray[i - 1];
  }

  /*printf("Original numbers:\n");
  for (int i = 0; i < answerCount; i++) {
    printf("%d ", numArray[i]);
  }
  printf("\nDifferences:\n");
  for (int i = 0; i < answerCount; i++) {
    printf("%d ", diffArray[i]);
  }
  printf("\n");*/

  bitStack *cValues = (bitStack *)malloc(sizeof(bitStack));
  cValues->size = 2;
  cValues->bits = (long *)malloc(sizeof(long) * cValues->size);
  cValues->index = 0;
  cValues->top = 0;

  clock_t startComp, endComp, startDecomp, endDecomp;
  startComp = clock();
  printf("Compressing...\n");
  compress(diffArray, answerCount, cValues);
  endComp = clock();

  printf("Decompressing...\n");

  startDecomp = clock();
  int *decompressedNumbers = decompress(cValues, answerCount);
  endDecomp = clock();

  /*printf("Decompressed numbers:\n");
   for (int i = 0; i < answerCount; i++) {
     printf("%d ", decompressedNumbers[i]);
   }*/
  double compTime, decompTime;
  compTime = (double)(endComp - startComp) / CLOCKS_PER_SEC;
  decompTime = (double)(endDecomp - startDecomp) / CLOCKS_PER_SEC;

  printf("Compression ratio is: %f\n", compressionRatio(cValues, answerCount));
  printf("Compression time: %f\n", compTime);
  printf("Decompression time: %f\n", decompTime);

  outputToFile(numArray, diffArray, decompressedNumbers, cValues, answerCount,
               compTime, decompTime);
}
