#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <vector>
#include <fstream>

#include "Solution.h"

typedef double LoadType;

/**
@brief Class contains math model parameters and initial solution
@detailed 
//TODO: add detailed description mb with math model in tex
*/

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
		@brief overload << to ease an output of conditions
	*/
	friend std::ostream &operator<< (std::ostream &ofs, const Condition &conditions);

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
		@brief the capacity of each server (Conditions#numberOfDisks)
		       for each resource (Conditions#numberOfCharacteristics)
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
	Solution initialSolution;
};


std::ostream &operator<<(std::ostream &ofs, const Condition &conditions);

#endif // CONDITIONS_H