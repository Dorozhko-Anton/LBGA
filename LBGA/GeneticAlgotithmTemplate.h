#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>
#include "Conditions.h"

namespace ice {
	template <class T>
	class Population {
	public:
		T* getBest() const {
			return population[0];
		}
		std::vector<T*> getCreaturesForCrossover();
		T* getCreatureForMutation();

		Population(std::vector<T*> _population) : population(_population){}
	private:
		std::vector<T*> population;
	};

	template <class T>
	class BreedingStrategy {
	public:
		std::vector<T*> generateInitialPopulation(int size) {
			return Solution::SequentialPopulationGeneration(conditions, size);
		};
		BreedingStrategy(const Condition* _conditions) {
			conditions = _conditions;
		};
	private:
		const Condition* conditions;
	};
}


template <
	class T,
	template<typename T> class Population,
	template<typename T> class InitialPopulationGenerator
>
class GeneticAlgorithm  {
public:
	void start() {
		population = new Population<T>(IPGSstrategy.generateInitialPopulation(populationSize));

		/*while (!stopCriteria()) {
			crossover();
			localSearch();
		}*/

	}

	T* getResult() const
	{
		return population->getBest();
	}

	GeneticAlgorithm(const Condition* const conditions,
		InitialPopulationGenerator<T> _generator
		) :
		IPGSstrategy(_generator) 
	{
	}

private:
	InitialPopulationGenerator<T> IPGSstrategy;
	/*StopCriteriaStrategy<T> stopCriteria;
	CrossoverStrategy<T> crossoverStrategy;
	LocalSearchStrategy<T> LSstrategy;
*/
	Population<T>* population;
	int populationSize = 50;
};

#endif //  GAT_H