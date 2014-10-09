#ifndef SOLUTION_H
#define SOLUTION_H
#pragma once

#include "Conditions.h"

//typedef double LoadType;



class Condition;
class Solution;
typedef std::vector<Solution *> Population;

class Solution
{
public:
	Solution();
	Solution(const Condition* const _problemConditions, std::ifstream &ifs);
	Solution(const Solution* const other);

	~Solution();

	/**
		check that constraints are not violated
	*/
	bool isFeasible() const;

	void calculateEjectionAndInsertionExpenses();
	void calculateOverLoad();

	/**
	  @brief generate population of size population_size
	         that will be distributed in the space of feasible 
			 solutions evenly
	  @detailed
	         metric - hamming distance
			 max_distance - Conditions#numberOfDisks
			 delta - max_distance*10/population_size ??? magic constant

			 idea: 
			 //////////////////////////////////////////////
			 popoulation <- initSolution
			 
			 
			 while population < population_size:
				newSolution <- copy(initSolution)
				while (hammDist(newSolution, population) < delta):
					newSolution.random_move(), random_swap()
				population <- newSolution
			//////////////////////////////////////////////
			we don't always can make newSolutions within delta from population
			so we need to make delta less or fill population with random 
			newSolutions upto population_size
	*/
	static std::vector<Solution *> SequentialPopulationGeneration(const Condition* const condition, int population_size);

	Solution pathRelinking(Solution * other) const;

	void assignmentLocalDescent();

	void localSearch();

	void randomMove();

	/**
		@brief counts hammDist from 1 solution to set of solutions called population
		       returns smallest dist 
	*/
	static int hammDistToPopulation(const Population population, const Solution * const solution);

	/**
		@brief computes hammdistance between two solutions
	*/
	int hammDist(const Solution * const other);

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