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


#endif // STOP_STRATEGY_H