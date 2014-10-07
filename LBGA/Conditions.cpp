#include "Conditions.h"

#include <fstream>

Condition::Condition(std::ifstream &inputFile) {

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
	
	initialSolution = new Solution(this, inputFile);
}

Condition::~Condition() {

}

std::ostream &operator<<(std::ostream &ofs, const Condition &Condition)
{
	ofs << Condition.getNumberOfDisks() << std::endl;
	ofs << Condition.getNumberOfServers() << std::endl;
	ofs << Condition.getNumberOfCharacteristics() << std::endl;
	ofs << Condition.getTimePeriod() << std::endl;
	for (int s = 0; s < Condition.getNumberOfServers(); s++) {
		for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
			ofs << Condition.getServerLoadLimits(s, c) << std::endl;
		}
	}
	for (int s = 0; s < Condition.getNumberOfServers(); s++) {
		for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
			ofs << Condition.getInsertionLimits(s, c) << std::endl;
		}
	}
	for (int s = 0; s < Condition.getNumberOfServers(); s++) {
		for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
			ofs << Condition.getEjectionLimits(s, c) << std::endl;
		}
	}

	for (int d = 0; d < Condition.getNumberOfDisks(); d++) {
		for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
			for (int t = 0; t < Condition.getTimePeriod(); t++) {
				ofs << Condition.getDiskLoad(d, c, t) << std::endl;
			}
		}
	}

	for (int d = 0; d < Condition.getNumberOfDisks(); d++) {
		for (int s = 0; s < Condition.getNumberOfServers(); s++) {
			for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
				ofs << Condition.getInsertionCost(d, s, c) << std::endl;
			}
		}
	}
	for (int d = 0; d < Condition.getNumberOfDisks(); d++) {
		for (int s = 0; s < Condition.getNumberOfServers(); s++) {
			for (int c = 0; c < Condition.getNumberOfCharacteristics(); c++) {
				ofs << Condition.getEjectionCost(d, s, c) << std::endl;
			}
		}
	}


	ofs << Condition.getInitSolution();

	return ofs;

}
