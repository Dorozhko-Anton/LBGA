#include <iostream>
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
	ice::IterationNumberStopStrategy<Solution> sstrategy(10);

	GeneticAlgorithm
		<Solution, 
		 ice::Population, 
		 ice::BreedingStrategy, 
		 ice::FastRandomGreedyStrategy,
		 ice::IterationNumberStopStrategy>
		 ga(&cond, bstrategy, sstrategy);

	ga.start();
	std::cout << ga.getResult()->getOverLoad() << std::endl;
}