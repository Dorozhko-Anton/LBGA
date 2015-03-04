#include <algorithm>
#include <random>
#include <numeric> // iota
#include "Solution.h"
#include "RandomNumberGenerator.h"

std::ostream &operator<<(std::ostream &ofs, const Solution &solution)
{
	//ofs << *(solution.problemConditions);
	for (std::vector< int >::const_iterator it = solution.solution.begin();
		it != solution.solution.end();
		it++)
	{
		ofs << *it + 1 << std::endl;
		// in real servers are numbered 1..numberOfservers;
	}
	return ofs;
}


Solution::Solution(const Condition* const _condition, std::ifstream &ifs, bool isInitSolution)
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


	if (!isInitSolution) {
		calculateEjectionAndInsertionExpenses();
	}
	calculateOverLoad();
}

bool Solution::operator==(const Solution const & other) const {
	return this->solution == other.solution;
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
			/*if (currentEjectionCost[s][c] > condition->getEjectionLimits(s, c)) {
				return false;
			}
			if (currentInsertionCost[s][c] > condition->getInsertionLimits(s, c)) {
				return false;
			}*/
			if (getOverallExpenses(s, c) > condition->getOverallLimits(s, c)) {
				return false;
			}
		}
	}
	return true;
}

std::vector<Solution *> Solution::RandomPopulationGeneration(const Condition* const condition, int population_size)
{
	std::vector<Solution *> population(0);
	population.push_back(new Solution(*condition->getInitSolution()));

	
	int moves = std::min(population_size, condition->getNumberOfDisks() / 10);

	while (population.size() < population_size)
	{
		Solution * new_solution = new Solution(*condition->getInitSolution());

		for (int i = 0; i < moves; i++) {
			new_solution->randomGoodSwap();
		}

		population.push_back(new_solution);
	}

	return std::move(population);
}

std::vector<Solution *> Solution::SequentialPopulationGeneration(const Condition* const condition, int population_size)
{
	std::vector<Solution *> population(0);
	population.push_back(condition->getInitSolution()->clone());

	int delta = std::min(population_size, condition->getNumberOfDisks() / 10);
	int attemptsMaxNumber;

	while (population.size() < population_size)
	{
		Solution * new_solution = condition->getInitSolution()->clone();


		attemptsMaxNumber = 2 * delta;
		while (hammDistToPopulation(population, new_solution) < delta && attemptsMaxNumber-- > 0)
		{
			new_solution->randomMove();
		}
		if (attemptsMaxNumber == 0) {
			new_solution->FastRandomizedGreedyOptimization(3);
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
	// TODO: BUG wrong if ejection and insertion are treated as one capacity
	return canEject(disk, solution[disk]) && canInsert(disk, server);
}

LoadType Solution::getOverallExpenses(int server, int characteristic) const {
	return currentEjectionCost[server][characteristic] + currentInsertionCost[server][characteristic];
}

bool Solution::canEject(int diskToMove, int serverToEjectDisk) const  {

	if (diskToMove == -1) {
		return true;
	}

	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
		if ((getOverallExpenses(serverToEjectDisk, c)
			+ condition->getEjectionCost(diskToMove, serverToEjectDisk, c))
	> condition->getOverallLimits(serverToEjectDisk, c))
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

	//TODO: change ?
	/*const Solution* initSolution = condition->getInitSolution();

	if (initSolution->solution[diskToMove] == serverToInsertDisk) {
		return true;
	}*/


	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
		if ((getOverallExpenses(serverToInsertDisk, c)
			+ condition->getInsertionCost(diskToMove, serverToInsertDisk, c))
	> condition->getOverallLimits(serverToInsertDisk, c)) {
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

void Solution::swap(int d1, int d2) {
	std::swap(solution[d1], solution[d2]);
	this->calculateEjectionAndInsertionExpenses();
	this->calculateOverLoad();
}

struct Move {
	int d;
	int s;

	Move(int _d, int _s ) : d(_d), s(_s) {}
};


void Solution::SwapOptimization() {

	std::pair<std::pair<int,int>, LoadType> bestMove(std::pair<int, int>(1, 1), 0);

	for (int d1 = 0; d1 < condition->getNumberOfDisks(); d1++) {
		for (int d2 = d1 + 1; d2 < condition->getNumberOfDisks(); d2++) {
			if (solution[d1] != solution[d2]) {
				std::pair<std::pair<int, int>, LoadType> tmp(std::pair<int, int>(d1, d2), trySwap(d1, d2));

				if (tmp.second > bestMove.second) {
					bestMove = tmp;
				}
			}
		}
	}

	swap(bestMove.first.first, bestMove.first.second);
	/*int numberOfNotImprovingIterations = 10;

	Solution * champ = new Solution(*this);
	Solution * curr = new Solution(*this);

	while (numberOfNotImprovingIterations > 0)
	{
		std::uniform_int_distribution<int> dist(0, condition->getNumberOfDisks() - 1);

		int d1 = dist(GlobalRNG::getInstance().getEngine());
		int d2 = dist(GlobalRNG::getInstance().getEngine());

		int s1 = solution[d1];
		int s2 = solution[d2];

		if (s1 != s2) {
			if (serversOverLoads[s1] == 0 ^ serversOverLoads[s2] == 0) {
				if (serversOverLoads[s1] == 0) {
					if (canMove(d2, s1)) {
						curr->move(d2, s1);
					}
				}
				else {
					if (canMove(d1, s2)) {
						curr->move(d1, s2);
					}
				}
			}
			else {
				if (serversOverLoads[s1] == 0 && serversOverLoads[s2] == 0) {
					numberOfNotImprovingIterations--; 
					continue;
				}
				else {
					if (canSwap(d1, d2)) {
						curr->swap(d1, d2);
					}
				}
			}


		}

		if (curr->getOverLoad() < champ->getOverLoad()) {
			delete champ;
			champ = new Solution(*curr);
		}
		else {
			numberOfNotImprovingIterations--;
		}
	}*/
}

void Solution::MoveOptimization() {
	std::pair<Move, LoadType> bestMove(Move(1, solution[1]), 0);

	for (int d = 0; d < condition->getNumberOfDisks(); d++) {
		for (int s = 0; s < condition->getNumberOfServers(); s++) {
			if (s != solution[d]) {
				std::pair<Move, LoadType> tmp(Move(d, s), tryMove(d, s));

				if (tmp.second > bestMove.second) {
					bestMove = tmp;
				}
			}
		}
	}

	move(bestMove.first.d, bestMove.first.s);
}

void Solution::MoveSwapOptimization() {
	std::pair<Move, LoadType> bestMove(Move(1, solution[1]), 0);

	for (int d = 0; d < condition->getNumberOfDisks(); d++) {
		for (int s = 0; s < condition->getNumberOfServers(); s++) {
			if (s != solution[d]) {
				std::pair<Move, LoadType> tmp(Move(d, s), tryMove(d, s));

				if (tmp.second > bestMove.second) {
					bestMove = tmp;
				}
			}
		}
	}

	std::pair<std::pair<int, int>, LoadType> bestSwap(std::pair<int, int>(1, 1), 0);

	for (int d1 = 0; d1 < condition->getNumberOfDisks(); d1++) {
		for (int d2 = d1 + 1; d2 < condition->getNumberOfDisks(); d2++) {
			if (solution[d1] != solution[d2]) {
				std::pair<std::pair<int, int>, LoadType> tmp(std::pair<int, int>(d1, d2), trySwap(d1, d2));

				if (tmp.second > bestSwap.second) {
					bestSwap = tmp;
				}
			}
		}
	}

	if (bestMove.second > bestSwap.second) {
		move(bestMove.first.d, bestMove.first.s);
	}
	else {
		swap(bestSwap.first.first, bestSwap.first.second);
	}
}

void Solution::RandomizedSwapOptimization() {
	bool moveDone;

	do {
		moveDone = false;

		std::vector<int> serversByOverLoad = getServersByOverLoad();

		std::uniform_int_distribution<int> distMore(0, serversByOverLoad.size() / 2);
		std::uniform_int_distribution<int> distLess(serversByOverLoad.size() / 2 + 1, serversByOverLoad.size() - 1);

		int moreOverloadedServerIndex = distMore(GlobalRNG::getInstance().getEngine());
		int lessOverloadedServerIndex = distLess(GlobalRNG::getInstance().getEngine());

		int moreOverloadedServer = serversByOverLoad[moreOverloadedServerIndex];
		int lessOverloadedServer = serversByOverLoad[lessOverloadedServerIndex];

		std::vector<int> disks1 = chooseRandomDisksFromServer(condition->getNumberOfDisks(), moreOverloadedServer);
		std::vector<int> disks2 = chooseRandomDisksFromServer(condition->getNumberOfDisks(), lessOverloadedServer);


		LoadType maxWin = 0;
		int disk1, disk2;

		// if server with disk2 has 0 overload => try move
		if (serversOverLoads[lessOverloadedServer] == 0) {
			for (int i = 0; i < disks1.size(); i++) {
				LoadType win = tryMove(disks1[i], lessOverloadedServer);

				if (win > maxWin) {
					disk1 = disks1[i];
					maxWin = win;
				}
			}
			if (maxWin > 0) {
				move(disk1, lessOverloadedServer);
				moveDone = true;
			}

		}
		else {
			for (int i = 0; i < disks1.size(); i++) {
				for (int j = 0; j < disks2.size(); j++) {
					LoadType win = trySwap(disks1[i], disks2[j]);

					if (win > maxWin) {
						disk1 = disks1[i];
						disk2 = disks2[j];
						maxWin = win;
					}
				}
			}
			if (maxWin > 0) {
				swap(disk1, disk2);
				moveDone = true;
			}
		}
	} while (moveDone);
}

void Solution::randomizedSwapOpt1() {
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		this->swap2good();
	}
}

void Solution::randomGoodSwap() {

	std::vector<int> serversByOverLoad = getServersByOverLoad();

	std::uniform_int_distribution<int> distMore(0, serversByOverLoad.size() / 2);
	std::uniform_int_distribution<int> distLess(serversByOverLoad.size() / 2 + 1, serversByOverLoad.size() - 1);

	int moreOverloadedServerIndex = distMore(GlobalRNG::getInstance().getEngine());
	int lessOverloadedServerIndex = distLess(GlobalRNG::getInstance().getEngine());

	int moreOverloadedServer = serversByOverLoad[moreOverloadedServerIndex];
	int lessOverloadedServer = serversByOverLoad[lessOverloadedServerIndex];

	std::vector<int> disks1 = chooseRandomDisksFromServer(condition->getNumberOfDisks(), moreOverloadedServer);
	std::vector<int> disks2 = chooseRandomDisksFromServer(condition->getNumberOfDisks(), lessOverloadedServer);

	int d1 = disks1[0];
	int d2 = disks2[0];

	if (trySwap(d1, d2) > 0) {
		swap(d1, d2);
	}
}

void Solution::swap2() {
	std::uniform_int_distribution<int> uniDist(0, condition->getNumberOfDisks());

	int d1 = uniDist(GlobalRNG::getInstance().getEngine());
	int d2 = uniDist(GlobalRNG::getInstance().getEngine());

	if (d1 != d2 && solution[d1] != solution[d2] && canSwap(d1, d2)) {
		swap(d1, d2);
	}
}

void Solution::swap2good() {
	std::uniform_int_distribution<int> uniDist(0, condition->getNumberOfDisks() - 1);

	int d1 = uniDist(GlobalRNG::getInstance().getEngine());
	int d2 = uniDist(GlobalRNG::getInstance().getEngine());

	if (d1 != d2 && solution[d1] != solution[d2] && trySwap(d1, d2) > 0) {
		swap(d1, d2);
	}
}


std::vector<Solution*> Solution::pathRelinking(Solution * other) const
{
	std::vector<Solution *> svec;

	Solution * s = this->clone();

	/*for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		if (solution[i] != other->solution[i]) {
			if (canMove(i, other->solution[i])) {
				s->move(i, other->solution[i]);
				Solution * tmp = new Solution(*s);
				tmp->RandomizedSwapOptimization();
				svec.push_back(tmp);
			}
		}
	}*/
	std::vector<int> diff;
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		if (this->solution[i] != other->solution[i]) {
			diff.push_back(i);
		}
	}

	std::random_shuffle(diff.begin(), diff.end());

	for (int i = 0; i < diff.size(); i++) {
			if (canMove(i, other->solution[i])) {
				s->move(i, other->solution[i]);
				Solution * tmp = s->clone();
				tmp->FastRandomizedGreedyOptimization(3);
				svec.push_back(tmp);
			}
	}


	std::sort(svec.begin(), svec.end(), [](Solution *a, Solution *b) {
		return a->getOverLoad() < b->getOverLoad();
	});
	if (svec.size() > 5) {
		for (int i = 5; i < svec.size(); i++) {
			delete svec[i];
		}
		svec.resize(5);
	}
	delete s;

	return std::move(svec);
}

Solution * Solution::clone() const {
	return new Solution(*this);
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
		else {
			moveDone = false;
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
				result += std::max((LoadType)0, serversLoads[server][c][t] + condition->getDiskLoad(disk, c, t) - condition->getServerLoadLimits(server, c));
			}
		}
		// was - become
		return (serversOverLoads[serverWas] + serversOverLoads[server]) - result;
	}
	
	return -1 * std::numeric_limits<LoadType>::infinity();

}

LoadType Solution::trySwap(int d1, int d2) const {
	// check constraints
	if (canSwap(d1, d2)) {
		int s1 = solution[d1];
		int s2 = solution[d2];
		// recalculate overLoad for two servers
		// for server to eject disk from - without that disk
		// for server to insert disk to - with that disk
		LoadType result = 0;

		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
			for (int t = 0; t < condition->getTimePeriod(); t++) {
				result += std::max((LoadType)0, 
					serversLoads[s1][c][t] 
					- condition->getDiskLoad(d1, c, t) 
					+ condition->getDiskLoad(d2, c, t) 
					- condition->getServerLoadLimits(s1, c));

				result += std::max((LoadType)0, 
					serversLoads[s2][c][t] 
					+ condition->getDiskLoad(d1, c, t) 
					- condition->getDiskLoad(d2, c, t)
					- condition->getServerLoadLimits(s2, c));
			}
		}
		// was - become
		return (serversOverLoads[s1] + serversOverLoads[s2]) - result;
	}

	return -1 * std::numeric_limits<LoadType>::infinity();
}

bool Solution::canSwap(int d1, int d2) const {

	int s1 = solution[d1];
	int s2 = solution[d2];

	bool canEjectD1 = true, canEjectD2 = true;
	bool canInsertD1 = true, canInsertD2 = true;

	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
		
		if (s2 != condition->getInitSolution()->solution[d1]) {
			canEjectD1 = canEjectD1 && (getOverallExpenses(s1, c)
				+ condition->getEjectionCost(d1, s1, c) < condition->getOverallLimits(s1, c));

			canInsertD1 = canInsertD1 &&
				(getOverallExpenses(s1, c) + condition->getInsertionCost(d2, s1, c) < condition->getOverallLimits(s1, c));
		}

		if (s1 != condition->getInitSolution()->solution[d2]) {
			canEjectD2 = canEjectD2 && (getOverallExpenses(s2, c)
				+ condition->getEjectionCost(d2, s2, c) < condition->getOverallLimits(s2, c));

			canInsertD2 = canInsertD2 &&
				(getOverallExpenses(s2, c) + condition->getInsertionCost(d1, s2, c) < condition->getOverallLimits(s2, c));
		}
	}
	
	return canEjectD1 && canEjectD2 && canInsertD1 && canInsertD2;
}



std::vector<int> solveMinAssignmentProblem(std::vector<std::vector<double>> a) {
	double INF = INF_LOAD;
	int n = a.size() - 1;
	int m = a.size() - 1;

	std::vector<long long> u(n + 1), p(m + 1), way(m + 1);
	std::vector<double> v(m + 1);
	for (int i = 1; i <= n; ++i) {
		p[0] = i;
		int j0 = 0;
		std::vector<double> minv(m + 1, INF);
		std::vector<char> used(m + 1, false);
		do {
			used[j0] = true;
			int i0 = p[j0], j1 = 0;
			double delta = INF;
			for (int j = 1; j <= m; ++j)
			if (!used[j]) {
				int cur = a[i0][j] - u[i0] - v[j];
				if (cur < minv[j])
					minv[j] = cur, way[j] = j0;
				if (minv[j] < delta)
					delta = minv[j], j1 = j;
			}
			for (int j = 0; j <= m; ++j)
			if (used[j])
				u[p[j]] += delta, v[j] -= delta;
			else
				minv[j] -= delta;
			j0 = j1;
		} while (p[j0] != 0);
		do {
			int j1 = way[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}

	std::vector<int> ans(n + 1);
	for (int j = 1; j <= m; ++j)
		ans[p[j]] = j;

	ans[0] = -v[0];

	return ans;
}


void Solution::LocalOptAsAssignmentProblem()
{
	// TODO:
	//
	int S = condition->getNumberOfServers();
	std::vector < int > disks(S, 0);


	// from each server eject 1 disk ( it is "work" in initial formulation of assignment problem)
	for (int i = 0; i < S; i++) {
		disks[i] = ejectRandomDiskFromServer(i);
	}

	// evaluate C[i][j] - weight matrix
	std::vector< std::vector < LoadType > > C =
		std::vector<std::vector < LoadType >>(S + 1, std::vector <LoadType>(S + 1));


	for (int i = 0; i < S + 1; i++) {
		for (int j = 0; j < S + 1; j++) {
			if (i == 0 || j == 0) {
				C[i][j] = 0;
			}
			else {
				
				if (canInsert(disks[i - 1], j - 1)) {
					C[i][j] = tryInsertDiskToServer(disks[i - 1], j - 1);
				}
				else {
					C[i][j] = INF_LOAD;
				}
			}
		}
	}

	std::vector < int > newPlacement(S + 1, 0);
	newPlacement = solveMinAssignmentProblem(C);

	for (int i = 1; i < newPlacement.size(); i++) {
		if (disks[i - 1] != -1) {
			//std::cout << "move" << disks[i - 1] << " to " << newPlacement[i] - 1 << std::endl;
			solution[disks[i - 1]] = newPlacement[i] - 1;
			//move(disks[i - 1], newPlacement[i] - 1);
		}
		
	}
	newPlacement.clear();
	calculateEjectionAndInsertionExpenses();
	calculateOverLoad();
}

int Solution::ejectRandomDiskFromServer(int server) {
	std::vector<int> allDisks;
	// write out all disks that belong to server
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		if (solution[i] == server) {
			allDisks.push_back(i);
		}
	}

	std::uniform_int_distribution<int> dist(0, allDisks.size());

	int disk = dist(GlobalRNG::getInstance().getEngine());

	if (canEject(disk, server)) {
		solution[disk] = -1;
		return disk;
	}

	return -1;
}
LoadType Solution::tryInsertDiskToServer(int disk, int server) {
	LoadType resultServerOverLoad = 0;
	LoadType serverLoad = 0;

	if (disk == -1) {
		return serversOverLoads[server];
	}

	for (int c = 0; c < condition->getNumberOfCharacteristics(); c++)
	{
		for (int t = 0; t < condition->getTimePeriod(); t++)
		{
			serverLoad = serversLoads[server][c][t] +
				condition->getDiskLoad(disk, c, t);

			int add = serverLoad
				- condition->getServerLoadLimits(server, c);
			resultServerOverLoad += add > 0 ? add : 0;
		}

	}
	return resultServerOverLoad;
}