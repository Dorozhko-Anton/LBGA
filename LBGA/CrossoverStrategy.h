#ifndef CROSSOVER_STRATEGY_H
#define CROSSOVER_STRATEGY_H
#pragma once

#include <vector>

template <
	class T
>
class CrossoverStrategy {
public:
	virtual std::vector<T*> crossover(std::pair<T*, T*> parents) = 0;
};

template <
	class T
>
class SimpleCrossoverStrategy : public CrossoverStrategy<T> {
public:
	std::vector<T*> crossover(std::pair<T*, T*> parents) {
		std::vector<T*> nextGen = parents.first->crossover(parents.second);
		return std::move(nextGen);
	}
};

#endif //CROSSOVER_STRATEGY_H