#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>

namespace ice {
	template <typename T>
	class Population {
	public:
		T getBest() const {
			return population[0];
		}
		std::vector<T> getCreaturesForCrossover();
		T getCreatureForMutation();


	private:
		std::vector<T> population;
	};

}


template <
	typename T,
	typename Population,
	typename InitialPopulationGenerator
>
class GeneticAlgorithm  {
public:
	void start() {
		IPGSstrategy.generateInitialPopulation();

		/*while (!stopCriteria()) {
			crossover();
			localSearch();
		}*/

	}

	T getResult() const
	{
		return population.getBest();
	}

private:
	InitialPopulationGenerator IPGSstrategy;
	/*StopCriteriaStrategy<T> stopCriteria;
	CrossoverStrategy<T> crossoverStrategy;
	LocalSearchStrategy<T> LSstrategy;
*/
	Population population;
};

#endif //  GAT_H