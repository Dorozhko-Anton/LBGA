#include <iostream>
#include "GeneticAlgotithmTemplate.h"
#include "Conditions.h"
#include "Solution.h"



void main() 
{
	using namespace ice;

	std::ifstream dataFile("data_700_150.txt");

	Condition cond(dataFile);
	ice::BreedingStrategy<Solution> bstrategy(&cond);

	GeneticAlgorithm<Solution, ice::Population, ice::BreedingStrategy, ice::FastRandomGreedyStrategy>
		ga(&cond, bstrategy);

	ga.start();
	std::cout << ga.getResult()->getOverLoad() << std::endl;
}