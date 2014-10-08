#ifndef Condition_H
#define Condition_H

#include <vector>
#include <fstream>

typedef int LoadType;

#include "Solution.h"



/**
@brief Class contains math model parameters and initial solution
@detailed 
//TODO: add detailed description mb with math model in tex
*/

class Solution;

class Condition
{
public:
	
	/**
		@brief read condition from file
		@detailed 
		  read size parameters
		  read server limits (load, insertion, ejection)
		  read disks params (load, insertion/ejection load)

		  call Solution#Solution(std::ifstream& inputFile)
	*/
	Condition(std::ifstream& inputFile);

	/**
		@brief overload << to ease an output of Condition
	*/
	friend std::ostream &operator<< (std::ostream &ofs, const Condition &Condition);

	inline int getNumberOfDisks() const;
	inline int getNumberOfServers() const;
	inline int getNumberOfCharacteristics() const;
	inline int getTimePeriod() const;
	inline LoadType getServerLoadLimits(int server, int characteristic) const;
	inline LoadType getInsertionLimits(int server, int characteristic) const;
	inline LoadType getEjectionLimits(int server, int characteristic) const;

	inline LoadType getDiskLoad(int disk, int characteristic, int time) const;
	inline LoadType getInsertionCost(int disk, int server, int characteristic) const;
	inline LoadType getEjectionCost(int disk, int server, int characteristic) const;

	inline const Solution* getInitSolution() const;

	~Condition();
private:
	/**
		disk has load by types of Characteristics and time
	*/
	int numberOfDisks;
	/**
		servers has critical capacity of each Characteristic
	*/
	int numberOfServers;
	/**
		load characteristics. 
		might be CPU, RAM, ...
	*/
	int numberOfCharacteristics;
	/**
		time interval we have predictions of load to
	*/
	int timePeriod;

	/**
		@brief the capacity of each server (Condition#numberOfDisks)
		       for each resource (Condition#numberOfCharacteristics)
			   Using: [s][c]
	*/
	std::vector< std::vector<LoadType> > serverLoadLimits;

	/**
		@brief each servers has limits of characteristics for inserting
			   disks
			   Using: [s][c]
	*/
	std::vector< std::vector<LoadType> > insertionLimits;

	/**
		@brief each servers has limits of characteristics for ejecting
			   disks
			   Using: [s][c]
	*/
	std::vector< std::vector<LoadType> > ejectionLimits;

	/**
		@brief each disk has predicted load for every characteristic 
		       for each timepoint of timePeriod
				
			   Using: [d][c][t]
	*/
	std::vector< std::vector< std::vector<LoadType> > > diskLoad;

	/**
	@brief each disk has predicted load for inserting it into server

	Using: [d][s][c]
	*/
	std::vector< std::vector< std::vector<LoadType> > > insertionCosts;
	/**
	@brief each disk has predicted load for ejecting it from server


	Using: [d][s][c]
	*/
	std::vector< std::vector< std::vector<LoadType> > > ejectionCosts;


	//////////////////////////////////////////////////////////////////
	/**
		Initial solution
		Current disk locations on servers
	*/
	Solution* initialSolution;
};


std::ostream &operator<<(std::ostream &ofs, const Condition &Condition);

// getters
inline int Condition::getNumberOfDisks() const
{
	return numberOfDisks;
}
inline int Condition::getNumberOfServers() const
{
	return numberOfServers;
}
inline int Condition::getNumberOfCharacteristics() const
{
	return numberOfCharacteristics;
}
inline int Condition::getTimePeriod() const
{
	return timePeriod;
}
inline LoadType Condition::getServerLoadLimits(int server, int characteristic) const
{
	return serverLoadLimits[server][characteristic];
}
inline LoadType Condition::getInsertionLimits(int server, int characteristic) const
{
	return insertionLimits[server][characteristic];
}
inline LoadType Condition::getEjectionLimits(int server, int characteristic) const
{
	return ejectionLimits[server][characteristic];
}
inline LoadType Condition::getDiskLoad(int disk, int characteristic, int time) const
{
	return diskLoad[disk][characteristic][time];
}
inline LoadType Condition::getInsertionCost(int disk, int server, int characteristic) const
{
	return insertionCosts[disk][server][characteristic];
}
inline LoadType Condition::getEjectionCost(int disk, int server, int characteristic) const
{
	return ejectionCosts[disk][server][characteristic];
}

inline const Solution* Condition::getInitSolution() const {
	return initialSolution;
}

#endif // Condition_H