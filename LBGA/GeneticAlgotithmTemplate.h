#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include "Conditions.h"
#include "RandomNumberGenerator.h"

namespace ice {

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

		std::pair<T*, T*> getCreaturesForCrossover() {
			std::sort(population.begin(), population.end(),
				[](T* a, T* b) {
				return (a->getOverLoad() - b->getOverLoad()) < 0;
			});

			std::uniform_int_distribution<int> dist(0, population.size() - 1);
			
			int parent1 = dist(GlobalRNG::getInstance().getEngine());
			int parent2 = dist(GlobalRNG::getInstance().getEngine());

			return std::pair<T*, T*>(population[parent1], population[parent2]);
		}

		T* getCreatureForMutation();


		void apply(LocalSearchStrategy<T>& const lsstrategy) {
			for (std::vector<T*>::iterator p = population.begin(); p != population.end(); p++) {
				lsstrategy.apply(*p);
			}
		};

		void add(Population<T>* other) {
			population.insert(population.end(), other->population.begin(), other->population.end());
		}

		void shrink() {
			// DRY violation add helper method for sorting or keep sorted continuously?
			// proirity queue ? sorted tree? 
			std::sort(population.begin(), population.end(),
				[](T* a, T* b) {
				return (a->getOverLoad() - b->getOverLoad()) < 0;
			});

			population.resize(populationStartSize);
		}

		Population(std::vector<T*> _population) 
			: population(_population),
			populationStartSize(_population.size())
			{}
	private:
		int populationStartSize;
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
	class SwapStrategy : public LocalSearchStrategy<T> {
	public:
		void apply(T* solution) {
			
			solution->SwapOptimization();
		};
	};

	template <class T>
	class RandomizedSwapStrategy : public LocalSearchStrategy<T> {
	public:
		void apply(T* solution) {

			solution->RandomizedSwapOptimization();
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

	template <
		class T
	>
	class CrossoverStrategy {
	public:
		virtual std::vector<T*> crossover(std::pair<T*, T*> parents) = 0;
	};

	template <
		class T
	>
	class SimpleCrossoverStrategy: public CrossoverStrategy<T> {
	public:
		std::vector<T*> crossover(std::pair<T*, T*> parents) {
			std::vector<T*> nextGen = parents.first->crossover(parents.second);
			return std::move(nextGen);
		}
	};
}


template <
	class T,
	template<typename T> class Population,
	template<typename T> class InitialPopulationGenerator,
	template<typename T> class LocalSearchStrategy,
	template<typename T> class StopStrategy,
	template<typename T> class CrossoverStrategy
>
class GeneticAlgorithm {
public:
	void start() {
		// generate start population
		population = new Population<T>(IPGSstrategy.generateInitialPopulation(populationSize));
		// apply localSearch to population
		population->apply(LSstrategy);
		
		while (!_StopStrategy.stopCriteria()) {
			
			// chose solutions from population to crossover
			auto parents = population->getCreaturesForCrossover();
			Population<T>* newGeneration = new Population<T>(crossoverStrategy.crossover(parents));
			newGeneration->apply(LSstrategy);
			
			population->add(newGeneration);
			population->shrink();
			delete newGeneration;
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
		_StopStrategy(_stopCriteria),
		crossoverStrategy()
	{
	}

private:
	InitialPopulationGenerator<T> IPGSstrategy;
	LocalSearchStrategy<T> LSstrategy;
	StopStrategy<T> _StopStrategy;
	CrossoverStrategy<T> crossoverStrategy;


	Population<T>* population;
	int populationSize = 40;
};

#endif //  GAT_H