#include <iostream>
#include <ctime>
#include "GeneticAlgotithmTemplate.h"
#include "Conditions.h"
#include "Solution.h"

#include "Population.h"
#include "PopulationGenerationStrategy.h"
#include "LocalSearchStrategy.h"
#include "CrossoverStrategy.h"
#include "StopStrategy.h"

void main() 
{

	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);
	BreedingStrategy<Solution> bstrategy(&cond);
	IterationNumberStopStrategy<Solution> sstrategy(400);

	GeneticAlgorithm
		<Solution, 
		 strategies::Population, 
		 BreedingStrategy, 
		 //FastRandomGreedyStrategy,
		 //SwapStrategy,
		 RandomizedSwapStrategy,
		 IterationNumberStopStrategy,
		 SimpleCrossoverStrategy
		>
		 ga(&cond, bstrategy, sstrategy);

	std::clock_t start, finish;
	std::clock_t timeSpan;
	start = std::clock();
	ga.start();
	finish = std::clock();

	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << finish - start << std::endl;
}