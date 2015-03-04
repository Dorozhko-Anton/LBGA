#include <iostream>
#include <algorithm>
#include <random>
#include <set>

#include "SolutionWithClasses.h"
#include "RandomNumberGenerator.h"


SolutionWithClasses::SolutionWithClasses(const Condition* const _problemConditions, std::ifstream &ifs, bool isInitSolution) 
:Solution(_problemConditions, ifs, isInitSolution)
{
	/*this->solution.resize(0);
	for (int i = 0; i < condition->getNumberOfDisks(); i++) {
		double s = 0;
		ifs >> s;
		diskClasses.push_back(static_cast<int>(s));
	}*/
	this->diskClasses = std::vector<int>(condition->getNumberOfDisks(), 0);
	int numberOfRedDisks = 0;
	ifs >> numberOfRedDisks;

	for (int i = 0; i < numberOfRedDisks; i++) {
		double s = 0;
		ifs >> s;
		diskClasses[static_cast<int>(s)] = Red;
	}

	int numberOfYellowDisks = 0;
	ifs >> numberOfYellowDisks;

	for (int i = 0; i < numberOfRedDisks; i++) {
		double s = 0;
		ifs >> s;
		diskClasses[static_cast<int>(s)] = Yellow;
	}
}

bool SolutionWithClasses::CheckCompatibility() {

	std::vector<int> servers(condition->getNumberOfServers(), 0);
	int numberOfDisks = condition->getNumberOfDisks();

	for (int i = 0; i < numberOfDisks; i++) {
		switch (diskClasses[i]) {
		case LoadClass::Red:
			redServersSet.insert(solution[i]);
				servers[solution[i]]++;
				if (servers[solution[i]] > 1) {
					std::cout << "Solution is not compatible [2 BIG disk on server: " << i << " ]" << std::endl;
					return false;
				}
				
			break;
		}
	}

	for (int i = 0; i < condition->getNumberOfServers(); i++) {
		if (servers[i] > 0) {

			for (int r = 0; r < condition->getNumberOfCharacteristics(); r++) {

				LoadType overallEjectionCost = 0;
				for (int j = 0; j < numberOfDisks; j++) {
					if (solution[j] == i && diskClasses[j] != LoadClass::Red) {
						overallEjectionCost += condition->getEjectionCost(j, i, r);
					}
				}
				if (overallEjectionCost > condition->getOverallLimits(i, r)) {

					for (int d = 0; d < numberOfDisks; d++) {
						if (diskClasses[d] == LoadClass::Red && solution[d] == i) {
							std::cout << "Could not dedicate server " << i << " with red disk " << d << std::endl;
							return false;
						}
					}
				}
			}
			
		}
	}
	return true;
}


void SolutionWithClasses::Prepare() {
	DedicateRedServers();
	calculateOverLoad();
	
}

void SolutionWithClasses::DedicateRedServers() {
	int k = 0;

	for (auto it = redServersSet.begin(); it != redServersSet.end(); it++) {
		std::vector<int> redServerDisks = GetRedServersDisks(*it);
		// change conditions to not allow insert disks to RedServers
		Condition* cond = const_cast<Condition*>(condition);

		for (auto itd = redServerDisks.begin(); itd != redServerDisks.end(); itd++) {
			// make not stupid moves
			Move(*itd);
			k++;

			for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {		
				cond->insertionCosts[*itd][*it][c] = 1;
				cond->ejectionCosts[*itd][*it][c] = 0;
			}
		}

		for (int c = 0; c < condition->getNumberOfCharacteristics(); c++) {
			cond->ejectionLimits[*it][c] = 0;
			cond->insertionLimits[*it][c] = 0;
		}
	}

	std::cout << "Disks moved during dedicating RED servers = " << k << std::endl;
	
}

void SolutionWithClasses::calculateOverLoad() {
	Solution::calculateOverLoad();

	std::vector<int> numberOfYellowDisk(condition->getNumberOfServers(), 0);

	for (int d = 0; d < condition->getNumberOfDisks(); d++) {
		if (diskClasses[d] == LoadClass::Yellow) {
			numberOfYellowDisk[solution[d]]++;
		}
	}

	for (int s = 0; s < condition->getNumberOfServers(); s++) {
		solutionOverLoad += PENALTY_CONSTANT * std::max(numberOfYellowDisk[s] - 1, 0);
	}
}

void SolutionWithClasses::Move(int disk) {
	
		std::vector<int> serversToMoveTo(0);
		for (int s = 0; s < condition->getNumberOfServers(); s++) {
			if (canInsert(disk, s)) {
				serversToMoveTo.push_back(s); 
			}
		}
		std::shuffle(serversToMoveTo.begin(), serversToMoveTo.end(), GlobalRNG::getInstance().getEngine());
		move(disk, serversToMoveTo[0]);
}

std::vector<int> SolutionWithClasses::GetRedServersDisks(int server) {
	std::vector<int> result(0);

	for (int d = 0; d < condition->getNumberOfDisks(); d++) {
		if (solution[d] == server && diskClasses[d] != LoadClass::Red) {
			result.push_back(d);
		}
	}

	return std::move(result);
}