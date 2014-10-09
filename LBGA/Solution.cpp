#include <algorithm>
#include "Solution.h"

Solution::Solution(const Condition* const _condition, std::ifstream &ifs) 
{
	this->condition = _condition;
	this->solution.resize(0);
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		double s = 0;
		ifs >> s;
		solution.push_back(static_cast<int>(s - 1));
	}
	this->serversLoads.resize(condition->getNumberOfServers(),
		std::vector< std::vector< LoadType > >(condition->getNumberOfCharacteristics(),
		std::vector<LoadType>(condition->getTimePeriod(), 0)));
	this->serversOverLoads.resize(condition->getNumberOfServers(), 0);

	this->currentEjectionCost.resize(condition->getNumberOfServers(),
		std::vector< LoadType >(condition->getNumberOfCharacteristics(), 0));
	this->currentInsertionCost.resize(condition->getNumberOfServers(),
		std::vector< LoadType >(condition->getNumberOfCharacteristics(), 0));

	calculateEjectionAndInsertionExpenses();
	calculateOverLoad();
}

Solution::Solution(const Solution* const other)
{
	// TODO: copy constructor
}

void Solution::calculateOverLoad()
{
	// TODO: implement calculateOverLoad
	// calculate load for each server for each timestamp for each characteristic
	// calculate overload
	solutionOverLoad = 0;

	for (int s = 0; s < condition->getNumberOfServers(); s++)
	{
		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++)
		{
			for (int t = 0; t < condition->getTimePeriod(); t++)
			{
				serversLoads[s][c][t] = 0;
			}
		}
	}

	for (int d = 0; d < condition->getNumberOfDisks(); d++)
	{
		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++)
		{
			for (int t = 0; t < condition->getTimePeriod(); t++)
			{
				serversLoads[solution[d]][c][t] +=
					condition->getDiskLoad(d, c, t);
			}
		}
	}

	for (int s = 0; s < condition->getNumberOfServers(); s++)
	{
		serversOverLoads[s] = 0;

		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++)
		{
			for (int t = 0; t < condition->getTimePeriod(); t++)
			{
				LoadType overLoad = 
					std::max((LoadType)0, serversLoads[s][c][t] - condition->getServerLoadLimits(s, c));

				serversOverLoads[s] += overLoad;
			}
		}
		solutionOverLoad += serversOverLoads[s];
	}
}

void Solution::calculateEjectionAndInsertionExpenses()
{
	//TODO: calculateEjectionAndInsertionExpenses
	// calculate expenses relative to Condition#initialSolution
	for (int i = 0; i < condition->getNumberOfServers(); i++) {
		for (int j = 0; j < condition->getNumberOfCharacteristics(); j++) {
			currentEjectionCost[i][j] = 0;
			currentInsertionCost[i][j] = 0;
		}
	}
	const Solution * initSolution = condition->getInitSolution();
	for (int d = 0; d < condition->getNumberOfDisks(); d++) {
		int serverWas = initSolution->solution[d];
		int serverBecome = solution[d];

		if (serverBecome != serverWas) {
			for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
				currentEjectionCost[serverWas][c] += 
					condition->getEjectionCost(d, serverWas, c);
				if (serverBecome == -1) { continue; }
				currentInsertionCost[serverBecome][c] += 
					condition->getInsertionCost(d, serverBecome, c);
			}
		}
	}
}

bool Solution::isFeasible() const 
{
	for (int s = 0; s < condition->getNumberOfServers(); s++) {
		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
			if (currentEjectionCost[s][c] > condition->getEjectionLimits(s, c) + EPSILON) {
				return false;
			}
			if (currentInsertionCost[s][c] > condition->getInsertionLimits(s, c) + EPSILON) {
				return false;
			}
		}
	}
	return true;
}

std::vector<Solution *> Solution::SequentialPopulationGeneration(const Condition* const condition, int population_size)
{
	std::vector<Solution *> population(0);
	population.push_back(new Solution(condition->getInitSolution()));

	int delta = condition->getNumberOfDisks() * 10 / population_size;

	while (population.size() < population_size)
	{
		Solution * new_solution = new Solution(condition->getInitSolution());

		while (hammDistToPopulation(population, new_solution) < delta)
		{
			new_solution->randomMove();
		}

		population.push_back(new_solution);
	}

	return std::move(population);
}

int Solution::hammDistToPopulation(const Population population, const Solution * const solution)
{
	//TODO: impl
	return 0;
}

void Solution::randomMove()
{
	//TODO: impl
}

Solution Solution::pathRelinking(Solution * other) const
{
	return *this;
}

void Solution::assignmentLocalDescent()
{

}

void Solution::localSearch()
{

}