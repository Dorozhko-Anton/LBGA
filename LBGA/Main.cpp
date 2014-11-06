#include <iostream>
#include <ctime>
#include <string>
#include "GeneticAlgotithmTemplate.h"
#include "Conditions.h"
#include "Solution.h"

#include "Population.h"
#include "PopulationGenerationStrategy.h"
#include "LocalSearchStrategy.h"
#include "CrossoverStrategy.h"
#include "StopStrategy.h"

void checkSolution(std::string conditionFileName, std::string solutionFileName) {
	std::ifstream conditionFile(conditionFileName);
	std::ifstream solutionFile(solutionFileName);

	Condition cond(conditionFile);
	Solution solution(&cond, solutionFile);

	//std::cout << solution << std::endl;
	std::cout << solution.getOverLoad() << std::endl;

	if (solution.isFeasible()) {
		std::cout << "Solution from \"" << solutionFileName << "\" is OK. Overload = " << solution.getOverLoad() << std::endl;
	}
	else {
		std::cout << "Solution from \"" << solutionFileName << "\" is BAD" << std::endl;
	}

	conditionFile.close();
	solutionFile.close();
}

void testGA() {
	std::ifstream dataFile("data_700_150.txt");
	//	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond);
	IterationNumberStopStrategy<Solution> sstrategy(100);

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
	start = std::clock();
	ga.start();
	finish = std::clock();

	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << finish - start << std::endl;
}

void main() 
{
	checkSolution("inst1.txt", "champ.txt");
	//testGA();
}