#ifndef _SOLUTION_WITH_PENALTY_
#define _SOLUTION_WITH_PENALTY_

#include "Solution.h"
#include "ConditionsWithPenalty.h"

class ConditionWithPenalty;

// idea: permit nonfeasible solutions, but add penalties in overload calculations

class SolutionWithPenalty : public Solution
{
public:
	SolutionWithPenalty(const ConditionWithPenalty* const _problemConditions, std::ifstream &ifs, bool isInitSolution)
	:Solution(_problemConditions, ifs, isInitSolution)
	{}


	SolutionWithPenalty(const Solution &solution) 
		:Solution(solution)
	{
		penalty = 500000;
	}

	virtual void calculateOverLoad() {
		Solution::calculateOverLoad();

		for (size_t s = 0; s < condition->getNumberOfServers(); s++)
		{
			for (size_t c = 0; c < condition->getNumberOfCharacteristics(); c++)
			{
				LoadType violation = currentEjectionCost[s][c] + currentInsertionCost[s][c] - condition->getOverallLimits(s, c);
				if (violation > 0) {
					solutionOverLoad += penalty * violation;
				}
			}
		}
		
	}


	Solution * clone() const {
		return new SolutionWithPenalty(*this);
	}

	bool canSwap(int d1, int d2) const {
		return true;
	}

	bool canMove(int disk, int server) const {
		return true;
	}

	void setPenalty(LoadType value = 10000) {
		penalty = value;
	}
	LoadType getPenalty() {
		return penalty;
	}
private:
	LoadType penalty;
};
#endif //_SOLUTION_WITH_PENALTY_
