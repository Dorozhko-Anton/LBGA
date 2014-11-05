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
		// generate start mPopulation
		mPopulation = new Population<T>(mInitialPopulationGenerator.generateInitialPopulation(mPopulationSize));
		// apply localSearch to mPopulation
		mPopulation->apply(mLocalSearchStrategy);
		
		while (!mStopStrategy.stopCriteria()) {
			
			// chose solutions from mPopulation to crossover
			auto parents = mPopulation->getCreaturesForCrossover();
			Population<T>* newGeneration = new Population<T>(mCrossoverStrategy.crossover(parents));
			newGeneration->apply(mLocalSearchStrategy);
			
			mPopulation->add(newGeneration);
			mPopulation->shrink();
			mPopulation->apply(mLocalSearchStrategy);
			delete newGeneration;
		}

	}

	T* getResult() const {
		return mPopulation->getBest();
	}

	GeneticAlgorithm(
		const Condition* const conditions,
		InitialPopulationGenerator<T> _generator,
		StopStrategy<T> _stopCriteria
	) :
		mInitialPopulationGenerator(_generator),
		mLocalSearchStrategy(),
		mStopStrategy(_stopCriteria),
		mCrossoverStrategy()
	{
	}

private:
	InitialPopulationGenerator<T> mInitialPopulationGenerator;
	LocalSearchStrategy<T> mLocalSearchStrategy;
	StopStrategy<T> mStopStrategy;
	CrossoverStrategy<T> mCrossoverStrategy;


	Population<T>* mPopulation;
	int mPopulationSize = 30;
};

#endif //  GAT_H