#ifndef SOLUTION_H
#define SOLUTION_H
#pragma once

#include "Conditions.h"

//typedef double LoadType;

class Condition;

class Solution
{
public:
	Solution();
	Solution(const Condition* const _problemConditions, std::ifstream &ifs);
	~Solution();

	/**
		check that constraints are not violated
	*/
	bool isFeasible() const;

	void calculateEjectionAndInsertionExpenses();
	void calculateOverLoad();


	static std::vector<Solution *> SequentialPopulationGeneration(int population_size);

	Solution pathRelinking(Solution * other) const;

	void assignmentLocalDescent();

	void localSearch();





private:
	/**
		@brief problem conditions. see Condition
	*/
	const Condition* condition;

	/**
		@brief solution is represented as vector of servers
		       solution[d] = s means disk d is on server s
	*/
	std::vector<int> solution;

	/////////////////////////////////////////////////////////
	// solution characteristics                            //
	/////////////////////////////////////////////////////////

	/**
	 @brief objective function 
	 @detailed
	 TODO: describe how to calculate
	*/
	LoadType solutionOverLoad;

	/**
	 ejection cost could not be greater than Condition#ejectionLimits	
	 usage: [s][c]
	*/
	std::vector< std::vector<LoadType> > currentEjectionCost;

	/**
	ejection cost could not be greater than Condition#insertionLimits
	usage: [s][c]
	*/
	std::vector< std::vector<LoadType> > currentInsertionCost;

	/////////////////////////////////////////////////////////
	// additional structures to recompute overloads easier //
	/////////////////////////////////////////////////////////

	/**
	 cache overload for each server, to recompute overload only 
	 on modified servers
	*/
	std::vector<LoadType> serversOverLoads;

	/**
	 cache server loads to compute modifications easier 
	 example: If we move disk from server. It is enough to subtract it's load
	 for every characteristic for every time unit, and recompute overload.
	 So we should not compute every disk load on server 

	 usage: [s][c][t]
	*/
	std::vector< std::vector< std::vector<LoadType> > > serversLoads;


};

const LoadType EPSILON = 10e-6;

#endif // SOLUTION_H