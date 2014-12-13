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
	Solution(){

	}

	bool operator==(const Solution const & other) const;
	Solution(const Condition* const _problemConditions, std::ifstream &ifs, bool isInitSolution = false);

	~Solution(){

	}

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


	/**
		@brief generate stating population randomly, but track the limitations
	*/
	static std::vector<Solution *> RandomPopulationGeneration(const Condition* const condition, int population_size);


	std::vector<Solution *> crossover(Solution * other) const {
		return pathRelinking(other);
	}

	std::vector<Solution*> pathRelinking(Solution * other) const;

	void LocalOptAsAssignmentProblem();
	//helpers
	int ejectRandomDiskFromServer(int server);
	LoadType tryInsertDiskToServer(int disk, int server);


	void localSearch();

	/**
		@brief simple move local search

		@detailed 
		while (move performed) do
			1) from MOST overloaded servers pick one
			2) choose @numberOfDisks random disks from it
			3) list moves that move disks to non-overloaded servers
				that decrease overload
			4) choose best
		end

	*/
	void FastRandomizedGreedyOptimization(int numberOfDisks);
	
	/**
		swap neighbourhood
		1. choose most overloaded server
		2. choose least overloaded server
		3. trySwap() all pairs and choose, pair that lowers the overload better
	*/
	void SwapOptimization();

	/**
	 make random swap if only it decrease overload
	*/
	void randomGoodSwap();


	/**
		while swap done 
			sort servers by overload
			choose first server from first half, second server from second half 
			if swap improving objective function 
			do swap.
	*/
	void RandomizedSwapOptimization();

	/**
		extensive Solution#swap2good 
	*/
	void randomizedSwapOpt1();

	/**
		
	*/
	void randomizedSwapOpt2();
	
	

	/**
		swap OPt helper functions
	*/
	void swap(int d1, int d2);
	LoadType trySwap(int d1, int d2) const;
	bool canSwap(int d1, int d2) const;

	/**
		@brief choose 2 disks. if they are not on the same server and canSwap => swap them
	*/
	void swap2();

	/**
		@brief swap2 but with additional check for improving objective function
	*/
	void swap2good();


	std::vector<int> getServersByOverLoad() const;
	std::vector<int> chooseRandomDisksFromServer(int amount, int server) const;
	LoadType tryMove(int disk, int server) const;

	/**
		chose disk uniformely
		chose server uniformely
		check constraints of moving
		move disk to server
	*/
	void randomMove();

	/**
		check constraints after move:
		1. canEjectDiskFromServer
		2. canInsertDiskToServer

		but without actually moving disk
	*/
	bool canMove(int disk, int server) const;

	/**
		check ejecting constraints of move
	*/
	bool canEject(int diskToMove, int serverToEjectDisk) const;
	/**
	    check inserting constraints of move
	*/
	bool canInsert(int diskToMove, int serverToInsertDisk) const;
	/**
		perform moving disk to new server

		update constraints
		update overload
	*/
	void move(int disk, int server);

	/**
		@brief counts hammDist from 1 solution to set of solutions called population
		       returns smallest dist 
	*/
	static int hammDistToPopulation(const Population population, const Solution * const solution);

	/**
		@brief computes hammdistance between two solutions
	*/
	int hammDist(const Solution * const other) const;

	/**
	getter for objective function
	*/
	LoadType getOverLoad() const {
		return solutionOverLoad;
	}

	/**
	get overall expenses
	*/
	LoadType getOverallExpenses(int server, int characteristic) const;

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

	friend std::ostream &operator<<(std::ostream &ofs, const Solution &solution);
};
std::ostream &operator<<(std::ostream &ofs, const Solution &solution);

const LoadType EPSILON = 10e-6;
const LoadType INF_LOAD = 10e6;

#endif // SOLUTION_H