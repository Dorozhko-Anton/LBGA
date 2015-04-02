#ifndef _CONDITION_WITH_HIGH_LOAD_H
#define _CONDITION_WITH_HIGH_LOAD_H

#include "Conditions.h"


class ConditionWithHighLoad : public Condition
{
public:
	ConditionWithHighLoad(std::ifstream& inputFile);
	~ConditionWithHighLoad();

private:

};

ConditionWithHighLoad::ConditionWithHighLoad(std::ifstream& inputFile)
{
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

	// HACK transport limits in file are common, and I used sum of ins end ejection limits
	// so read limits in insert and null eject to achieve equivalence

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
			ejectionLimits[s][c] = 0;
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
	
	initialSolution = new Solution(this, inputFile, true);
}

ConditionWithHighLoad::~ConditionWithHighLoad() {
	
}



#endif // _CONDITION_WITH_HIGH_LOAD_H