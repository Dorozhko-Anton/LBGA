#include <iostream>
#include "GeneticAlgotithmTemplate.h"
#include "Solution.h"

template <typename T>
class BreedingStrategy {
public:
	std::vector<T> generateInitialPopulation() {
		return std::vector<T>(0);
	}
};

void main() 
{
	using namespace ice;
	GeneticAlgorithm<Solution, ice::Population<Solution>, BreedingStrategy<Solution>> ga;

	ga.start();
	std::cout << ga.getResult().getOverLoad() << std::endl;
}