#ifndef LS_STRATEGY_H
#define LS_STRATEGY_H
#pragma once


template <class T>
class LocalSearchStrategy {
public:
	virtual void apply(T*) = 0;
};


template <class T>
class FastRandomGreedyStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		// TODO: get rid of magic constant
		solution->FastRandomizedGreedyOptimization(3);
	};
};

template <class T>
class SwapStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->SwapOptimization();
	};
};

template <class T>
class RandomizedSwapStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->RandomizedSwapOptimization();
	};
};

template <class T>
class ExtensiveRandomizedSwapStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->randomizedSwapOpt1();
	};
};

template <class T>
class MoveStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->MoveOptimization();
	};
};

template <class T>
class RandomizedMoveStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->RandomizedMoveOptimization();
	};
};

template <class T>
class MoveSwapStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->MoveSwapOptimization();
	};
};




template <class T>
class RandomizedMoveSwapStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->RandomizedMoveSwapOptimization();
	};
};

template <class T>
class LinKernighanStrategy : public LocalSearchStrategy<T> {
public:
	void apply(T* solution) {
		solution->LinKernighan(50);
	};
};




#endif //LS_STRATEGY_H