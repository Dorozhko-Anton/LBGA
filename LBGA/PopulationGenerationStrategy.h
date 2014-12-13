#ifndef PG_STRATEGY_H
#define PG_STRATEGY_H
#pragma once

template <class T>
class BreedingStrategy {
public:
	std::vector<T*> generateInitialPopulation(int size) {
		return T::SequentialPopulationGeneration(conditions, size);
	};
	BreedingStrategy(const Condition* _conditions) {
		conditions = _conditions;
	};
private:
	const Condition* conditions;
};


template <class T>
class RandomBreedingStrategy {
public:
	std::vector<T*> generateInitialPopulation(int size) {
		return T::RandomPopulationGeneration(conditions, size);
	};
	RandomBreedingStrategy(const Condition* _conditions) {
		conditions = _conditions;
	};
private:
	const Condition* conditions;
};


#endif // PG_STRATEGY_H