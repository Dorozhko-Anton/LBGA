#ifndef POPULATION_H
#define POPULATION_H
#pragma once
#include "LocalSearchStrategy.h"

namespace strategies {
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
}

#endif // POPULATION_H