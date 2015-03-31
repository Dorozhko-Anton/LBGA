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

private:
	void generateInitialPopulation() {
		// generate start mPopulation
		mPopulation = new Population<T>(mInitialPopulationGenerator.generateInitialPopulation());
		// apply localSearch to mPopulation
		mPopulation->apply(mLocalSearchStrategy);
	}
	void crossoverStage(){
		// chose solutions from mPopulation to crossover
		auto parents = mPopulation->getCreaturesForCrossover();
		std::shared_ptr<Population<T>> newGeneration(new Population<T>(mCrossoverStrategy.crossover(parents)));
			
		
		newGeneration->apply(mLocalSearchStrategy);

		mPopulation->add(newGeneration);
		//mPopulation->apply(mLocalSearchStrategy);
		
	}
	void mutationStage() {

	}
	void selection() {
		mPopulation->shrink();
	}

public:


	void start() {
		logfstream << "start" << std::endl;

		generateInitialPopulation();

		while (!mStopStrategy.stopCriteria()) {
			
			mPopulation->revalidate();

			crossoverStage();
			mutationStage();

			selection();
			
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
		InitialPopulationGenerator<T> _generator,
		StopStrategy<T> _stopCriteria
	) :
		mInitialPopulationGenerator(_generator),
		mLocalSearchStrategy(),
		mStopStrategy(_stopCriteria),
		mCrossoverStrategy()
	{}

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
	

	//
	std::ofstream logfstream;
};

#endif //  GAT_H