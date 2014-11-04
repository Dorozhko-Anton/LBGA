#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include "Conditions.h"
#include "RandomNumberGenerator.h"


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
			population->apply(LSstrategy);
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
	int populationSize = 30;
};

#endif //  GAT_H