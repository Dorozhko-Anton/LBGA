#ifndef _CONDITIONS_WITH_PENALTY_
#define _CONDITIONS_WITH_PENALTY_

#pragma once

#include "Conditions.h"
#include "Solution.h"
#include "SolutionWithPenalty.h"

class SolutionWithPenalty;

class ConditionWithPenalty : public Condition
{
public:
	ConditionWithPenalty(std::ifstream &inputFile) {
		double tmp;
		inputFile >> tmp;
		numberOfDisks = static_cast<int> (tmp);

		//inputFile >> numberOfServers;
		inputFile >> tmp;
		numberOfServers = static_cast<int> (tmp);

		//inputFile >> numberOfCharacteristics;
		inputFile >> tmp;
		numberOfCharacteristics = static_cast<int> (tmp);
		//inputFile >> timePeriod;
		inputFile >> tmp;
		timePeriod = static_cast<int> (tmp);

		// serverLoadLimits
		serverLoadLimits.resize(numberOfServers,
			std::vector<LoadType>(numberOfCharacteristics, 0));
		for (int s = 0; s < numberOfServers; s++) {
			for (int c = 0; c < numberOfCharacteristics; c++) {
				inputFile >> serverLoadLimits[s][c];
			}
		}
		// insertLimits
		insertionLimits.resize(numberOfServers,
			std::vector<LoadType>(numberOfCharacteristics, 0));
		for (int s = 0; s < numberOfServers; s++) {
			for (int c = 0; c < numberOfCharacteristics; c++) {
				inputFile >> insertionLimits[s][c];
			}
		}
		// ejectionLimits
		ejectionLimits.resize(numberOfServers,
			std::vector<LoadType>(numberOfCharacteristics, 0));
		for (int s = 0; s < numberOfServers; s++) {
			for (int c = 0; c < numberOfCharacteristics; c++) {
				inputFile >> ejectionLimits[s][c];
			}
		}
		// discLoad
		diskLoad.resize(numberOfDisks,
			std::vector< std::vector<LoadType> >(numberOfCharacteristics,
			std::vector<LoadType>(timePeriod, 0)));
		for (int d = 0; d < numberOfDisks; d++) {
			for (int c = 0; c < numberOfCharacteristics; c++) {
				for (int t = 0; t < timePeriod; t++) {
					inputFile >> diskLoad[d][c][t];
				}
			}
		}
		// insertionCosts
		insertionCosts.resize(numberOfDisks,
			std::vector< std::vector<LoadType> >(numberOfServers,
			std::vector<LoadType>(numberOfCharacteristics, 0)));
		for (int d = 0; d < numberOfDisks; d++) {
			for (int s = 0; s < numberOfServers; s++) {
				for (int c = 0; c < numberOfCharacteristics; c++) {
					inputFile >> insertionCosts[d][s][c];
				}
			}
		}
		// ejectionCosts
		ejectionCosts.resize(numberOfDisks,
			std::vector< std::vector<LoadType> >(numberOfServers,
			std::vector<LoadType>(numberOfCharacteristics, 0)));
		for (int d = 0; d < numberOfDisks; d++) {
			for (int s = 0; s < numberOfServers; s++) {
				for (int c = 0; c < numberOfCharacteristics; c++) {
					inputFile >> ejectionCosts[d][s][c];
				}
			}
		}
	
		Solution* solution = new Solution(this, inputFile, true);

		initialSolution = solution;
	}

	//bool Prepare() {
	//	SolutionWithClasses* solution = static_cast<SolutionWithClasses*>(initialSolution);
	//	if (solution->CheckCompatibility()) {
	//		solution->Prepare();
	//		return true;
	//	};

	//	return false;
	//}
	~ConditionWithPenalty() {}

private:

	friend class SolutionWithPenalty;
};

#endif // _CONDITIONS_WITH_PENALTY_