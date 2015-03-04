#ifndef GAT_H
#define GAT_H
#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <iomanip>

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
		logfstream << "start" << std::endl;

		while (!mStopStrategy.stopCriteria()) {
			
			// chose solutions from mPopulation to crossover
			auto parents = mPopulation->getCreaturesForCrossover();
			Population<T>* newGeneration = new Population<T>(mCrossoverStrategy.crossover(parents));
			newGeneration->apply(mLocalSearchStrategy);
			
			mPopulation->add(newGeneration);
			mPopulation->shrink();
			mPopulation->apply(mLocalSearchStrategy);
			delete newGeneration;

			
			logfstream << std::fixed << std::setw(11) << std::setprecision(6)
				<< mPopulation->getBest()->getOverLoad() << std::endl;
			std::cout << std::fixed << std::setw(11) << std::setprecision(6)
				<< mPopulation->getBest()->getOverLoad() << std::endl;
		}

	}

	std::shared_ptr<T> getResult() const {
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
		// generate start mPopulation
		mPopulation = new Population<T>(mInitialPopulationGenerator.generateInitialPopulation(mPopulationSize));
		// apply localSearch to mPopulation
		mPopulation->apply(mLocalSearchStrategy);
	}

	~GeneticAlgorithm() {
		delete mPopulation;
		logfstream.close();
	}

	void setLog(std::string filename) {
		logfstream = std::ofstream(filename, std::ios::app);
	}

private:
	InitialPopulationGenerator<T> mInitialPopulationGenerator;
	LocalSearchStrategy<T> mLocalSearchStrategy;
	StopStrategy<T> mStopStrategy;
	CrossoverStrategy<T> mCrossoverStrategy;


	Population<T>* mPopulation;
	int mPopulationSize = 100;

	//
	std::ofstream logfstream;
};

#endif //  GAT_H