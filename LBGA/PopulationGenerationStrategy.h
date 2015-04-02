#ifndef PG_STRATEGY_H
#define PG_STRATEGY_H
#pragma once

template <class T>
class BreedingStrategy {
public:
	std::vector<T*> generateInitialPopulation() {
		return T::SequentialPopulationGeneration(_conditions, _size);
	};
	BreedingStrategy(const Condition* conditions, int size) 
		: _conditions(conditions), _size(size)
	{};
private:
	const Condition* _conditions;
	int _size;
};


template <class T>
class RandomBreedingStrategy {
public:
	std::vector<T*> generateInitialPopulation() {
		return T::RandomPopulationGeneration(_conditions, _size);
	};
	RandomBreedingStrategy(const Condition* conditions, int size)
		: _conditions(conditions), _size(size)
	{};
private:
	const Condition* _conditions;
	int _size;
};


#endif // PG_STRATEGY_H