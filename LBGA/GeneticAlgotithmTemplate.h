#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>
#include <algorithm>
#include "Conditions.h"

namespace ice {

	//template <class T> 
	//class LocalSearchStrategy;

	template <class T>
	class LocalSearchStrategy {
	public:
		virtual void apply(T*) = 0;
	};

	template <class T>
	class Population {
	public:
		T* getBest() {
			std::sort(population.begin(), population.end(), 
				[](T* a, T* b) {
					return (a->getOverLoad() - b->getOverLoad()) < 0;
			});
			return population[0];
		}

		std::vector<T*> getCreaturesForCrossover();
		T* getCreatureForMutation();


		void apply(LocalSearchStrategy<T>& const lsstrategy) {
			for (std::vector<T*>::iterator p = population.begin(); p != population.end(); p++) {
				lsstrategy.apply(*p);
			}
		};

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



	template <class T>
	class FastRandomGreedyStrategy: public LocalSearchStrategy<T> {
	public:
		void apply(T* solution) {
			// TODO: get rid of magic constant
			solution->FastRandomizedGreedyOptimization(3);
		};
	};

	template <class T>
	class StopStrategy {
	public:
		virtual bool stopCriteria() = 0;
	};


	template <class T>
	class IterationNumberStopStrategy : public StopStrategy<T> {
	public:
		bool stopCriteria() {
			return numberOfIterations-- == 0;
		}

		IterationNumberStopStrategy(int _number) : numberOfIterations(_number) {}
	private:
		int numberOfIterations;
	};
}


template <
	class T,
	template<typename T> class Population,
	template<typename T> class InitialPopulationGenerator,
	template<typename T> class LocalSearchStrategy,
	template<typename T> class StopStrategy
>
class GeneticAlgorithm {
public:
	void start() {
		population = new Population<T>(IPGSstrategy.generateInitialPopulation(populationSize));

		population->apply(LSstrategy);
		
		while (!_StopStrategy.stopCriteria()) {
			//crossover();
			//localSearch();
			population->apply(LSstrategy);
		}

	}

	T* getResult() const
	{
		return population->getBest();
	}

	GeneticAlgorithm(const Condition* const conditions,
		InitialPopulationGenerator<T> _generator,
		StopStrategy<T> _stopCriteria
		) :
		IPGSstrategy(_generator),
		LSstrategy(),
		_StopStrategy(_stopCriteria)
	{
	}

private:
	InitialPopulationGenerator<T> IPGSstrategy;
	LocalSearchStrategy<T> LSstrategy;
	StopStrategy<T> _StopStrategy;

	/*StopCriteriaStrategy<T> stopCriteria;
	CrossoverStrategy<T> crossoverStrategy;
	LocalSearchStrategy<T> LSstrategy;
*/
	Population<T>* population;
	int populationSize = 10;
};

#endif //  GAT_H