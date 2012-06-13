#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

bool ***populations;
unsigned int *fitness, gen, j, k, onBits, best, seed;
double *selprob, sum, avg, r;

/**
 * Select an inidividual according to the selection probabilities
 *
 * @return the index of the selected individual
 */
int selecti(char **params) {
  for (r = (double)rand() / RAND_MAX, k = 0; r >= 0.0 && k < atoi(params[2]); r -= selprob[k++]);
  return k-1;
}

/**
 * Dynamically allocate arrays
 */
void allocateNdArrays(void **array, long baseSize, int dimensions, int paramLevel, char **params) {
  *array = malloc(dimensions == 1 ? baseSize*atoi(params[paramLevel]) : sizeof(void *)* (paramLevel == 3 ? 2 : atoi(params[paramLevel])));
  int row; for (row = 0; row <= (paramLevel == 3 ? 2 : atoi(params[paramLevel])) && dimensions > 1; row++)
     allocateNdArrays(*array+row*sizeof(void *), baseSize, dimensions-1, paramLevel-1, params);
}

/**
 * Entry Point
 */
int main(int argc, char **args) {
  //Seed, if none provided use time
  srand(seed = (argc == 7 ? atoi(args[6]) : time(NULL)));

  //Set up data structures
  allocateNdArrays((void**)&populations, sizeof(bool), 3, 3, args); 
  allocateNdArrays((void*)&fitness, sizeof(int), 1, 2, args); 
  allocateNdArrays((void*)&selprob, sizeof(double), 1, 2, args);

  //Init population
  for (j = 0; j < atoi(args[2])*atoi(args[1]); j++)
    populations[0][j / atoi(args[1])][j % atoi(args[1])] = rand() % 2;

  //Print Headers
  printf("Length: %d\nPop size: %d\nGenerations: %d\nCrossover prob.: %.2f\nMut prob.: %.2f\nSeed: %d\n", atoi(params[1]), atoi(params[2]), atoi(params[3]), atoi(params[4])/100.0, atoi(params[5])/100.0, seed);
  printf("Generation     Avg. Fitness   Best Fitness   Best Individual\n");

  //Generation Loop, break if successful
  for (gen = 0; gen <= atoi(args[3]) && (gen > 0 ? fitness[best] > 0 : 1); gen++) {
    //Calculate fitness of each individual, as well as accumulating the sum of fitnesses and the average fitness
    for (avg = 0.0, sum = 0.0, best = 0, j = 0; j < atoi(args[2]); sum += fitness[j], avg += ((double)fitness[j])/atoi(args[2]), ++j) {
      //Count bits and subtract from problem size
      for (onBits = 0, k = 0; k < atoi(args[1]); onBits += populations[gen % 2][j][k++]);
      fitness[j] = atoi(args[1]) - onBits;
      //Store the index of the best individual
      best = fitness[j] < fitness[best] ? j : best;
    }

    //Calculate the selection probabilities using the fitness values
    for (j = 0; j < atoi(args[2]); j++)
      selprob[j] = (atoi(args[1])-fitness[j])/(atoi(args[1])*atoi(args[2])-sum);

    //Output the stats
    printf("%-15d%-15f%-15d", gen, avg, fitness[best]);
    for (j = 0; j < atoi(args[1]); j++)
      printf("%d", populations[gen % 2][best][j]);
    printf("\n");

    //For the popsize, reproduce unless a cross over is required
    for (j = 0; j < atoi(args[2]); j++) {
      memcpy(populations[(gen+1) % 2][j], populations[gen % 2][selecti(args)], sizeof(bool)*atoi(args[1]));
      memcpy(populations[(gen+1) % 2][j], populations[gen % 2][selecti(args)], (rand() % 100 < atoi(args[4])) ? sizeof(bool)*(rand() % atoi(args[1])) : 0);
      //Mutate the new individual
      for (k = 0; k < atoi(args[1]); k++)
        populations[(gen+1) % 2][j][k] = rand() % 100 < atoi(args[5]) ? rand() % 2 : populations[(gen+1) % 2][j][k];
    }
  }
  printf(fitness[best] > 0 ? "Finished\n" : "Success!\n");
  return 0;
}
