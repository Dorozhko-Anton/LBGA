#ifndef POPULATION_H
#define POPULATION_H
#pragma once

#include <memory>
#include "LocalSearchStrategy.h"
#include "RandomNumberGenerator.h"

namespace strategies {
	template <class T>
	class Population {
	public:
		std::shared_ptr<T> getBest() {
			sortByOverLoad();
			return mPopulation[0];
		}

		std::pair<std::shared_ptr<T>, std::shared_ptr<T>> getCreaturesForCrossover() {
			sortByOverLoad();

			std::uniform_int_distribution<int> dist(0, mPopulation.size() - 1);

			int parent1 = dist(GlobalRNG::getInstance().getEngine());
			int parent2 = dist(GlobalRNG::getInstance().getEngine());

			return std::pair<std::shared_ptr<T>, std::shared_ptr<T>>(mPopulation[parent1], mPopulation[parent2]);
		}

		std::shared_ptr<T> getCreatureForMutation();


		void apply(LocalSearchStrategy<T>& const lsstrategy) {
			for (auto p = mPopulation.begin(); p != mPopulation.end(); p++) {
				lsstrategy.apply(&**p);
			}
		};

		//HACK
		void revalidate() {
			for (auto p = mPopulation.begin(); p != mPopulation.end(); p++) {
				(*p)->calculateOverLoad();
			}
		};

		void add(std::shared_ptr<Population<T>> other) {
			mPopulation.insert(mPopulation.end(), other->mPopulation.begin(), other->mPopulation.end());

			mPopulation.erase(std::unique(mPopulation.begin(), mPopulation.end()), mPopulation.end());
			//std::cout << mPopulation.size() << std::endl;
		}

		void shrink() {
			sortByOverLoad();
			mPopulation.resize(mPopulationStartSize);
		}

		Population(std::vector<T*> _population)
			: mPopulationStartSize(_population.size())
		{
			for (auto i = _population.begin(); i != _population.end(); i++)
			{
				mPopulation.push_back(std::shared_ptr<T>(*i));
			}
		}

		~Population() {
			mPopulation.clear();
		}

	private:
		void sortByOverLoad() {
			std::sort(mPopulation.begin(), mPopulation.end(),
				[](std::shared_ptr<T> a, std::shared_ptr<T> b) {
				return (a->getOverLoad() - b->getOverLoad()) < 0;
			});
		}

		int mPopulationStartSize;
		std::vector<std::shared_ptr<T>> mPopulation;
	};
}

#endif // POPULATION_H