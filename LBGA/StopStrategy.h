#ifndef STOP_STRATEGY_H
#define STOP_STRATEGY_H
#pragma once

template <class T>
class StopStrategy {
public:
	virtual bool stopCriteria() = 0;
};


template <class T>
class IterationNumberStopStrategy : public StopStrategy<T> {
public:
	bool stopCriteria() {
		return numberOfIterations-- == 0;
	}

	IterationNumberStopStrategy(int _number) : numberOfIterations(_number) {}
private:
	int numberOfIterations;
};


#include "Conditions.h"

template <class T>
class IterationNumberStopStrategyWithPenalty : public StopStrategy<T> {
public:
	bool stopCriteria() {

		condition->setPenalty(finalPenalty - numberOfIterations * step);

		return numberOfIterations-- == 0;
	}

	IterationNumberStopStrategyWithPenalty(Condition* _condition, int _number, LoadType _startPenalty, LoadType _finalPenalty)
		: numberOfIterations(_number), startPenalty(_startPenalty), finalPenalty(_finalPenalty)
	{
		condition = _condition;
		step = (finalPenalty - startPenalty) / numberOfIterations;
	}
private:
	Condition* condition;
	int numberOfIterations;
	LoadType startPenalty;
	LoadType finalPenalty;

	LoadType step;
};


#endif // STOP_STRATEGY_H