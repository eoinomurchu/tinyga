#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool ***populations;
int *fitness, gen, j, k, onBits, best;
double *selprob, sum, avg, r;
char **params;

int selecti() {
  for (r = (double)rand() / RAND_MAX, k = 0; r >= 0.0 && k < atoi(params[2]); r -= selprob[k++]);
  return k-1;
}

void xo_and_rep_then_mut() {
  for (j = 0; j < atoi(params[2]); j++) {
    memcpy(populations[(gen+1) % 2][j], populations[gen % 2][selecti()], sizeof(bool)*atoi(params[1]));
    memcpy(populations[(gen+1) % 2][j], populations[gen % 2][selecti()], (rand() % 100 < atoi(params[4])) ? sizeof(bool)*(rand() % atoi(params[1])) : 0);
    for (k = 0; k < atoi(params[1]); k++)
      populations[(gen+1) % 2][j][k] = rand() % 100 < atoi(params[5]) ? rand() % 2 : populations[(gen+1) % 2][j][k];
  }
}

void fitnessFunction() {
  for (avg = 0.0, sum = 0.0, best = 0, j = 0; j < atoi(params[2]); sum += fitness[j], avg += ((double)fitness[j])/atoi(params[2]), ++j) {
    for (onBits = 0, k = 0; k < atoi(params[1]); onBits += populations[gen % 2][j][k++]);
    fitness[j] = atoi(params[1]) - onBits;
    best = fitness[j] < fitness[best] ? j : best;
  }
  for (j = 0; j < atoi(params[2]); j++)
    selprob[j] = (atoi(params[1])-fitness[j])/(atoi(params[1])*atoi(params[2])-sum);
}

void print() {
  printf("%-15d%-15f%-15d", gen, avg, fitness[best]);
  for (j = 0; j < atoi(params[1]); j++)
    printf("%d", populations[gen % 2][best][j]);
  printf("\n");
}

void allocateNdArrays(void **array, long baseSize, int dimensions, int paramLevel) {
  *array = malloc(dimensions == 1 ? baseSize*atoi(params[paramLevel]) : sizeof(void *)* (paramLevel == 3 ? 2 : atoi(params[paramLevel])));
  int row; for (row = 0; row <= (paramLevel == 3 ? 2 : atoi(params[paramLevel])) && dimensions > 1; row++)
    allocateNdArrays(*array+row*sizeof(void *), baseSize, dimensions-1, paramLevel-1);
}

/* 1 Len  2 Popsize  3 Gens  4 xo  5 mut  6 seed */
int main(int argc, char **args) {
  params = args;
  srand(atoi(params[6]));
  allocateNdArrays((void**)&populations, sizeof(bool), 3, 3); 
  allocateNdArrays((void*)&fitness, sizeof(int), 1, 2); 
  allocateNdArrays((void*)&selprob, sizeof(double), 1, 2);
  for (j = 0; j < atoi(params[2])*atoi(params[1]); j++)
    populations[0][j / atoi(params[1])][j % atoi(params[1])] = rand() % 2;
  printf("Generation     Avg. Fitness   Best Fitness   Best Individual\n");
  for (gen = 0; gen <= atoi(params[3]) && (gen > 0 ? fitness[best] > 0 : 1); gen++) {
    fitnessFunction();
    print();
    xo_and_rep_then_mut();
  }
  printf(fitness[best] > 0 ? "Finished\n" : "Success!\n");
  return 0;
}
