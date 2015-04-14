#ifndef SOLUTION_WITH_CLASSES_H
#define SOLUTION_WITH_CLASSES_H

#pragma once

#include <vector>
#include <set>

#include "Solution.h"

class SolutionWithClasses : public Solution
{
public:

	SolutionWithClasses(const Condition* const _problemConditions, std::ifstream &ifs, bool isInitSolution = false);

	/**
		@brief sum up all ejection cost for disks with Big(Red) disk in server 
				and compare with overallLimits
	*/
	bool CheckCompatibility();

	/**
		@brief prepare solution for Genetic Algorithm
	*/
	void Prepare();

	/**
		@brief use some greed algorithm to unload Big(Red) servers
	*/
	void DedicateRedServers();


	/**
		@brief overload objective function to add penalty for Yellow disks
	*/
	virtual void calculateOverLoad();

	virtual ~SolutionWithClasses() {}

	Solution * clone() const {
		return new SolutionWithClasses(*this);
	}

private:
	const LoadType PENALTY_CONSTANT = 100000;
	enum LoadClass { Green, Yellow, Red };
	/**
		@brief disk classes : small(green), potentially big(yellow), big(red)
	*/
	std::vector<int> diskClasses;

	std::set<int> redServersSet;


	////////
	// HELPERS
	////////

	std::vector<int> GetRedServersDisks(int server);

	void Move(int disk);

	//void calculateViolations();
};


#endif // SOLUTION_WITH_CLASSES_H