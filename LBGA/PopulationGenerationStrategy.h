#ifndef PG_STRATEGY_H
#define PG_STRATEGY_H
#pragma once

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


#endif // PG_STRATEGY_H