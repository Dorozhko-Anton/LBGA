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

void Solution::calculateOverLoad()
{
	// TODO: implement calculateOverLoad
	// calculate load for each server for each timestamp for each characteristic
	// calculate overload
}

void Solution::calculateEjectionAndInsertionExpenses()
{
	//TODO: calculateEjectionAndInsertionExpenses
	// calculate expenses relative to Condition#initialSolution
}