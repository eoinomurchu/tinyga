# tinyga #

A Tiny GA written in C as a submission to [The Murphy Cup](http://cup.murph.ie "The Murphy Cup")

## Usage ##

`./tinyga length popsize generations xo_percentage mutation_percentage random_seed`

## Description ##

The goal is to create as a short as possible \(but still comprehendible\) a fixed length binary [GA](http://en.wikipedia.org/wiki/Genetic_algorithm) to solve the [OneMax problem](http://tracer.lcc.uma.es/problems/onemax/onemax.html). The GA is requried to use crossover \(xo\), reproduction and mutation as genetic operators and [fitness proportionate selection](http://en.wikipedia.org/wiki/Fitness_proportionate_selection) for... selection.

Fitness/Error is minimising.

<code>rand</code> from <code>stdlib.h</code> is used throughout. The <code>params\[6\] is used as a seed using <code>srand</code>.

### Parameters ###
There are six command line arguments/parameters. <code>params</code> holds a pointer to them:
   * Problem or individual length
   * Population Size
   * Number of generations/iterations
   * Crossover probability/1\-Reproduction probability
   * Mutation probability
   * Random number generator seed

<code>atoi</code> is used throughout to convert the parameter strings to integer values.

### Data Structures ###

The data structures are as follows:
   * A 2\*PopSize\*IndiviualSize _Bool array to store the current generation's population and the next generation's population. These alternate using <code>mod 2</code>
   * A 2\*PopSize int array to store the fitness values
   * A 2\*PopSize double array to store the selection probabilities
   * Fitness and selection probability could be one dimensional of size PopSize I guess

### Allocation ###

The data structures are then allocated on the heap using the recursive <code>allocateNdArrays</code> function. 
   * The memory allocated at each recursive call depends on two parameters, <code>dimensions</code>, how many dimensions out of N are left to be allocated, and <code>paramLevel</code>, which index from <code>params</code> should read for the size of this current dimension
      * There are two cases for <code>dimensions</code>:
         * <code>&gt; 1</code> the data type size being allocated is that of a pointer
         * Otherwise, allocate the actual data type size, stored in <code>baseSize</code>
      * There are also two cases for <code>paramLevel</code>
         * <code>paramLevel == 3</code> i.e., generational level, only use size 2 for this dimension
         * Otherwise use size of <code>params\[paramLevel\]</code> for this dimension

### Initialisation ###

Initialisation is as follows:
   * Normally two for-loops but since size is an issue, the product of the two ranges are used as a new range for a single loop: <code>params\[2\] \* params\[1\]</code>
      * To extract the first variable from this range, divide by the second value
      * To extract the second variable from this range, mod by the second

### Generation Loop ###

Loop from generation 0 while less than the total number of generations and the best inidividual evaluated has a fitness &gt; 0.
   * Evaluate/Call fitness function
   * Print stats
   * Perform genetic operations

After exiting the loop, only perform the last evaluation if the final generation was reached. Otherwise Success!.

### Fitness Evaluation ###

Iterate over each individual of the population to assign fitness values
   * Calculate the fitness by iterating over the individual and counting the number of bits set to 1
      * The fitness value used is actually the error, which is minimised, so subtract this number from the length of inividual/problem size/<code>params\[1\]</code>
      * Also keep track of index of the most fit individual in <code>best</code>
      * Over the loop a number of variables are cumulated, the <code>sum</code> of fitness and the <code>avg</code>. fitness of the population

Next, the fitness values are used to assign selection probabilities, <code>selprob</code>
   * Iterating over each individual dividing the individual's fitness by the sum of all fitness values in the population
      * The number of bits set to 1 is used here, as opposed to the error, so some additional calculation is required.

### Print Stats ###

Outputs the current generation, average fitness in the population, best fitness in the population and a string representation of the most fit individual to the terminal.

### Selection ###
During fitness evaluation each individual is assigned a selection probability. A random number in the range \[0, 1\] is generated. Iterating through the population, the selection probability of each individual is subtracted from the random number. This continues until the random number is &lt; zero. The last individual iterated over is selected. 

### Genetic Operaions ###

   * An individual is selected from the population using <code>selecti</code> and is copied into the next generation's population
   * Another inidividual is selected
      * A number from the range \[0, 100\) is generated and compared against the crossover probability in <code>params\[4\]</code>: 
         * If &lt; this probability, another random number is chosen in the range [0, IndividualSize) and over the start of the previously copied individual \(crossover\)
         * Otherwise, nothing else is copied \(reproduction\)
   * This new individual is iterated over, checking each bit to see if mutation events should occur, <code>rand() % 100 < params\[5\]</code>