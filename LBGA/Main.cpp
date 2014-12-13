#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

//#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

#include "GeneticAlgotithmTemplate.h"
#include "Conditions.h"
#include "Solution.h"

#include "Population.h"
#include "PopulationGenerationStrategy.h"
#include "LocalSearchStrategy.h"
#include "CrossoverStrategy.h"
#include "StopStrategy.h"



void checkSolution(std::string conditionFileName, std::string solutionFileName) {
	std::ifstream conditionFile(conditionFileName);
	std::ifstream solutionFile(solutionFileName);

	Condition cond(conditionFile);
	Solution solution(&cond, solutionFile);

	//std::cout << solution << std::endl;
	std::cout << solution.getOverLoad() << std::endl;

	if (solution.isFeasible()) {
		std::cout << "Solution from \"" << solutionFileName << "\" is OK. Overload = " << solution.getOverLoad() << std::endl;
	}
	else {
		std::cout << "Solution from \"" << solutionFileName << "\" is BAD" << std::endl;
	}

	conditionFile.close();
	solutionFile.close();
}

void testGA() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond);
	RandomBreedingStrategy<Solution> rbstrategy(&cond);

	IterationNumberStopStrategy<Solution> sstrategy(100);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		SwapStrategy,
		IterationNumberStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(&cond, bstrategy, sstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;

	for (int i = 0; i < 10; i++) {
		ga.getResult()->LocalOptAsAssignmentProblem();
		std::cout << i << ": " << ga.getResult()->isFeasible() << ", " << ga.getResult()->getOverLoad() << std::endl;
	}
	std::cout << "time: " << (finish - start)/CLOCKS_PER_SEC << std::endl;
	std::cout << *ga.getResult() << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << ".txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
	
	
}

void main() 
{
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); // enable file output
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtMemState _ms;
	//_CrtMemCheckpoint(&_ms);
	//
	//checkSolution("inst1.txt", "champ.txt");
	testGA();

	//_CrtMemDumpAllObjectsSince(&_ms);
}