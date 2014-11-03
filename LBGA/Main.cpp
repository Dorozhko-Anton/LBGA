#include <iostream>
#include <ctime>
#include "GeneticAlgotithmTemplate.h"
#include "Conditions.h"
#include "Solution.h"



void main() 
{
	using namespace ice;

	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);
	ice::BreedingStrategy<Solution> bstrategy(&cond);
	ice::IterationNumberStopStrategy<Solution> sstrategy(400);

	GeneticAlgorithm
		<Solution, 
		 ice::Population, 
		 ice::BreedingStrategy, 
		 //ice::FastRandomGreedyStrategy,
		 //ice::SwapStrategy,
		 ice::RandomizedSwapStrategy,
		 ice::IterationNumberStopStrategy,
		 ice::SimpleCrossoverStrategy
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