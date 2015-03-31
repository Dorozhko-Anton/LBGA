#ifndef _SOLUTION_WITH_PENALTY_
#define _SOLUTION_WITH_PENALTY_

#include "Solution.h"


// idea: permit nonfeasible solutions, but add penalties in overload calculations

class SolutionWithPenalty : public Solution
{
public:
	SolutionWithPenalty(const Condition* const _problemConditions, std::ifstream &ifs, bool isInitSolution)
	:Solution(_problemConditions, ifs, isInitSolution)
	{}


	SolutionWithPenalty(const Solution &solution) 
		:Solution(solution)
	{}

	virtual void calculateOverLoad() {
		Solution::calculateOverLoad();

		for (size_t s = 0; s < condition->getNumberOfServers(); s++)
		{
			for (size_t c = 0; c < condition->getNumberOfCharacteristics(); c++)
			{
				LoadType violation = currentEjectionCost[s][c] + currentInsertionCost[s][c] - condition->getOverallLimits(s, c);
				if (violation > 0) {
					solutionOverLoad += getPenalty() * violation;
					//std::cout << "server: " << s << "penalty: " << violation << std::endl;
				}
			}
		}
		
	}


	virtual Solution * clone() const {
		return new SolutionWithPenalty(*this);
	}

	virtual bool canSwap(int d1, int d2) const {
		return true;
	}

	virtual bool canMove(int disk, int server) const {
		return true;
	}

	virtual LoadType trySwap(int d1, int d2) const {
		int serverWas = solution[d1];
		int server = solution[d2];

		// recalculate violations for two servers
		LoadType violationsWin = 0;
		LoadType violations = 0;
		LoadType violationsBecome = 0;

		for (size_t c = 0; c < condition->getNumberOfCharacteristics(); c++)
		{
			LoadType wasExpenses1 = currentInsertionCost[serverWas][c]
				+ currentEjectionCost[serverWas][c];

			LoadType wasExpenses2 = currentInsertionCost[server][c]
				+ currentEjectionCost[server][c];

			if (wasExpenses1 > condition->getOverallLimits(serverWas, c)) {
				violations += wasExpenses1 - condition->getOverallLimits(serverWas, c);
			}

			if (wasExpenses2 > condition->getOverallLimits(server, c)) {
				violations += wasExpenses2 - condition->getOverallLimits(server, c);
			}

			// change

			wasExpenses1 += condition->getEjectionCost(d1, serverWas, c) - condition->getInsertionCost(d1, serverWas, c);
			wasExpenses1 += -(condition->getEjectionCost(d2, server, c) - condition->getInsertionCost(d2, server, c));

			wasExpenses2 += -(condition->getEjectionCost(d1, server, c) - condition->getInsertionCost(d1, server, c));
			wasExpenses2 += condition->getEjectionCost(d2, serverWas, c) - condition->getInsertionCost(d2, serverWas, c);

			if (wasExpenses1 > condition->getOverallLimits(serverWas, c)) {
				violationsBecome += wasExpenses1 - condition->getOverallLimits(serverWas, c);
			}

			if (wasExpenses2 > condition->getOverallLimits(server, c)) {
				violationsBecome += wasExpenses2 - condition->getOverallLimits(server, c);
			}

			violationsWin += violations - violationsBecome;
		}


		return Solution::trySwap(d1, d2) + violationsWin * getPenalty();
	}

	virtual LoadType tryMove(int disk, int server) const {
		
		int serverWas = solution[disk];
		// recalculate violations for two servers
		LoadType violationsWin = 0;
		LoadType violations = 0;
		LoadType violationsBecome = 0;

		for (size_t c = 0; c < condition->getNumberOfCharacteristics(); c++)
		{
			LoadType wasExpenses1 = currentInsertionCost[serverWas][c]
				+ currentEjectionCost[serverWas][c];
				
			LoadType wasExpenses2 = currentInsertionCost[server][c]
				+ currentEjectionCost[server][c];

			if (wasExpenses1 > condition->getOverallLimits(serverWas, c)) {
				violations += wasExpenses1 - condition->getOverallLimits(serverWas, c);
			}

			if (wasExpenses2 > condition->getOverallLimits(server, c)) {
				violations += wasExpenses2 - condition->getOverallLimits(server, c);
			}

			// change

			wasExpenses1 += condition->getEjectionCost(disk, serverWas, c) - condition->getInsertionCost(disk, serverWas, c);

			wasExpenses2 += -(condition->getEjectionCost(disk, server, c) - condition->getInsertionCost(disk, server, c));

			if (wasExpenses1 > condition->getOverallLimits(serverWas, c)) {
				violationsBecome += wasExpenses1 - condition->getOverallLimits(serverWas, c);
			}

			if (wasExpenses2 > condition->getOverallLimits(server, c)) {
				violationsBecome += wasExpenses2 - condition->getOverallLimits(server, c);
			}
			
			violationsWin += violations - violationsBecome;
		}
		
		
		return Solution::tryMove(disk, server) + violationsWin * getPenalty();
	}


	LoadType getPenalty() const {
		return condition->getPenalty();
	}
};
#endif //_SOLUTION_WITH_PENALTY_
