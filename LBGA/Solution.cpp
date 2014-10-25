#include <algorithm>
#include <random>
#include <numeric> // iota
#include "Solution.h"
#include "RandomNumberGenerator.h"

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

	//calculateEjectionAndInsertionExpenses();
	calculateOverLoad();
}



void Solution::calculateOverLoad()
{
	// implement calculateOverLoad
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
	//calculateEjectionAndInsertionExpenses
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
	population.push_back(new Solution(*condition->getInitSolution()));

	int delta = std::min(population_size, condition->getNumberOfDisks() / 10);

	while (population.size() < population_size)
	{
		Solution * new_solution = new Solution(*condition->getInitSolution());

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
	// fix distance init with max dist + 1
	int distance = solution->condition->getNumberOfDisks() + 1;

	for_each(population.begin(), population.end(), 
		[&](const Solution * elem) {
		  int currDist = solution->hammDist(elem);
		  // fix distance equals min distance between solution 
		  // and every other solution in population
		  if (currDist < distance) distance = currDist;
	    }
	);
	return distance;
}

int Solution::hammDist(const Solution * const other) const 
{
	int distance = 0;
	for (int i = 0; i < solution.size(); i++)
	{
		if (solution[i] != other->solution[i])
		{
			distance++;
		}
	}
	return distance;
}

void Solution::randomMove()
{
	//TODO: impl
	std::uniform_int_distribution<> disk_dist(0, solution.size() - 1);
	std::uniform_int_distribution<> server_dist(0, condition->getNumberOfServers() - 1);
	
	//dist(GlobalRND.getEngine());
	int disk = disk_dist(GlobalRNG::getInstance().getEngine());
	int server = server_dist(GlobalRNG::getInstance().getEngine());

	if (canMove(disk, server)) {
		move(disk, server);
	}
}

bool Solution::canMove(int disk, int server) const {
	return canEject(disk, solution[disk]) && canInsert(disk, server);
}

bool Solution::canEject(int diskToMove, int serverToEjectDisk) const  {

	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
		if ((currentEjectionCost[serverToEjectDisk][c]
			+ condition->getEjectionCost(diskToMove, serverToEjectDisk, c))
	> condition->getEjectionLimits(serverToEjectDisk, c))
		{

			return false;
		}
	}
	return true;
}

bool Solution::canInsert(int diskToMove, int serverToInsertDisk) const {
	// check if we can insert
	// that means not exceed insertionCostLimits
	if (diskToMove == -1) {
		return true;
	}

	const Solution* initSolution = condition->getInitSolution();

	if (initSolution->solution[diskToMove] == serverToInsertDisk) {
		return true;
	}


	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
		if ((currentInsertionCost[serverToInsertDisk][c]
			+ condition->getInsertionCost(diskToMove, serverToInsertDisk, c))
	> condition->getInsertionLimits(serverToInsertDisk, c)) {
			return false;
		}
	}
	return true;
}


void Solution::move(int disk, int server) {
	solution[disk] = server;
	this->calculateEjectionAndInsertionExpenses();
	this->calculateOverLoad();
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

void Solution::FastRandomizedGreedyOptimization(int numberOfRandomDisks /*= 0*/)
{
	bool moveDone = false;

	

	do
	{
		std::vector<int> serversByOverload = getServersByOverLoad();
		int mostOverloadedServer = serversByOverload[0];

		std::vector<int> disks = chooseRandomDisksFromServer(numberOfRandomDisks, mostOverloadedServer);

		LoadType maxWin = 0;
		int diskToMove = 0, serverForDisk = 0;
		for (std::vector<int>::iterator disk = disks.begin(); disk != disks.end(); disk++)
		{
			for (int s = serversByOverload.size() - 1; s >= serversByOverload.size() / 2; s--)
			{
				LoadType overLoadDecrease = tryMove(*disk, serversByOverload[s]);
				if (overLoadDecrease > maxWin) {
					diskToMove = *disk;
					serverForDisk = serversByOverload[s];
					maxWin = overLoadDecrease;
				}
			}
		}
		
		if (maxWin > 0) {
			moveDone = true;
			move(diskToMove, serverForDisk);
		}

	} while (moveDone);
}

std::vector<int> Solution::getServersByOverLoad() const {
	std::vector<int> result(condition->getNumberOfServers());
	// init by range 0..numberOfDisks - 1
	std::iota(result.begin(), result.end(), 0);

	// sort by descending order
	std::sort(result.begin(), result.end(), 
		[this](int i, int j) {
		return (serversOverLoads[i] - serversOverLoads[j]) > 0;
		}
	);
	return std::move(result);
}

std::vector<int> Solution::chooseRandomDisksFromServer(int amount, int server) const {
	

	std::vector<int> allDisks;
	// write out all disks that belong to server
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		if (solution[i] == server) {
			allDisks.push_back(i);
		}
	}


	if (allDisks.size() <= amount) {
		return std::move(allDisks);
	}
	
	// random shuffle disk numbers
	std::shuffle(allDisks.begin(), allDisks.end(), GlobalRNG::getInstance().getEngine());

	// get first #amount of disks
	std::vector<int> disks(allDisks.cbegin(), allDisks.cbegin() + amount);
	return std::move(disks);
}

LoadType Solution::tryMove(int disk, int server) const {
	
	// check constraints
	if (canMove(disk, server)) {
		int serverWas = solution[disk];
		// recalculate overLoad for two servers
		// for server to eject disk from - without that disk
		// for server to insert disk to - with that disk
		LoadType result = 0;

		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
			for (int t = 0; t < condition->getTimePeriod(); t++) {
				result += std::max((LoadType)0, serversLoads[serverWas][c][t] - condition->getDiskLoad(disk, c, t) - condition->getServerLoadLimits(serverWas, c));
				result += std::max((LoadType)0, serversLoads[server][c][t] - condition->getDiskLoad(disk, c, t) - condition->getServerLoadLimits(server, c));
			}
		}
		// was - become
		return (serversOverLoads[serverWas] + serversOverLoads[server]) - result;
	}
	
	return -1 * std::numeric_limits<LoadType>::infinity();

}