#ifndef POPULATION_H
#define POPULATION_H
#pragma once
#include "LocalSearchStrategy.h"
#include "RandomNumberGenerator.h"

namespace strategies {
	template <class T>
	class Population {
	public:
		T* getBest() {
			sortByOverLoad();
			return mPopulation[0];
		}

		std::pair<T*, T*> getCreaturesForCrossover() {
			sortByOverLoad();

			std::uniform_int_distribution<int> dist(0, mPopulation.size() - 1);

			int parent1 = dist(GlobalRNG::getInstance().getEngine());
			int parent2 = dist(GlobalRNG::getInstance().getEngine());

			return std::pair<T*, T*>(mPopulation[parent1], mPopulation[parent2]);
		}

		T* getCreatureForMutation();


		void apply(LocalSearchStrategy<T>& const lsstrategy) {
			for (std::vector<T*>::iterator p = mPopulation.begin(); p != mPopulation.end(); p++) {
				lsstrategy.apply(*p);
			}
		};

		void add(Population<T>* other) {
			mPopulation.insert(mPopulation.end(), other->mPopulation.begin(), other->mPopulation.end());

			mPopulation.erase(std::unique(mPopulation.begin(), mPopulation.end()), mPopulation.end());
			std::cout << mPopulation.size() << std::endl;
		}

		void shrink() {
			sortByOverLoad();
			mPopulation.resize(mPopulationStartSize);
		}

		Population(std::vector<T*> _population)
			: mPopulation(_population),
			mPopulationStartSize(_population.size())
		{}
	private:
		void sortByOverLoad() {
			std::sort(mPopulation.begin(), mPopulation.end(),
				[](T* a, T* b) {
				return (a->getOverLoad() - b->getOverLoad()) < 0;
			});
		}

		int mPopulationStartSize;
		std::vector<T*> mPopulation;
	};
}

#endif // POPULATION_H