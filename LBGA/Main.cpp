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
#include "SolutionWithClasses.h"
#include "SolutionWithPenalty.h"

#include "Population.h"
#include "PopulationGenerationStrategy.h"
#include "LocalSearchStrategy.h"
#include "CrossoverStrategy.h"
#include "StopStrategy.h"
#include "TimeStopStrategy.h"

#include "ConditionsWithClasses.h"
#include "ConditionsWithPenalty.h"
#include "ConditionWithHighLoad.h"


const int RECOMMENDED_POPULATION_SIZE = 30;
 int RECOMMENDED_TIME = 10 * 60;
const int RECOMMENDED_ITERATION_NUMBER = 500;

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


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		MoveSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start)/CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "union.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
void testGAwithRandomSwapMove() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		//MoveSwapStrategy,
		RandomizedMoveSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "RANDunion.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}

void testGAwithMove() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		MoveStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);


	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;

	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;
	

	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "move.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
void testGAwithSwap() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		SwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "swap.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}

void testGAwithRandomMove() {
	{
		//std::ifstream dataFile("data_700_150.txt");
		std::ifstream dataFile("inst1.txt");

		Condition cond(dataFile);

		BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
		IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
		TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

		GeneticAlgorithm
			<Solution,
			strategies::Population,
			BreedingStrategy,
			//RandomizedSwapStrategy,
			RandomizedMoveStrategy,
			//IterationNumberStopStrategy,
			TimeStopStrategy,
			SimpleCrossoverStrategy
			>
			ga(bstrategy, tsstrategy);


		ga.setLog("output.txt");

		std::clock_t start, finish;
		start = std::clock();
		ga.start();
		finish = std::clock();

		Solution * res = &*ga.getResult();
		res->calculateOverLoad();

		if (ga.getResult()->isFeasible()) {
			std::cout << "OK" << std::endl;
		}
		else {
			std::cout << "BAD" << std::endl;
		}
		std::cout << ga.getResult()->getOverLoad() << std::endl;

		std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


		if (ga.getResult()->isFeasible()) {

			std::stringstream outputStringStream;
			outputStringStream << ga.getResult()->getOverLoad() << "randommove.txt";
			std::cout << "Writing result to " << outputStringStream.str() << std::endl;

			std::ofstream result(outputStringStream.str());
			result << *ga.getResult() << std::endl;

		}
		else {
			std::cout << "Result is not feasible" << std::endl;
		}
	}
}
void testGAwithRandomSwap() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		RandomizedSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "randomswap.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
//
//
void testGAWithViolations() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	ConditionWithPenalty cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	RandomBreedingStrategy<Solution> rbstrategy(&cond, RECOMMENDED_POPULATION_SIZE);

	IterationNumberStopStrategy<Solution> sstrategy(50);

	IterationNumberStopStrategyWithPenalty<Solution> growPenalty(&cond, 50, 0, 1000);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		MoveSwapStrategy,
		//IterationNumberStopStrategy,
		IterationNumberStopStrategyWithPenalty,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, growPenalty);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;

	/*for (int i = 0; i < 10; i++) {
		ga.getResult()->LocalOptAsAssignmentProblem();
		std::cout << i << ": " << ga.getResult()->isFeasible() << ", " << ga.getResult()->getOverLoad() << std::endl;
	}*/
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;
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

void testLK() {
	/*std::ifstream dataFile("filesMigrationProblem25000.txt");
	std::ofstream output("LK25000.output");
	ConditionWithHighLoad cond(dataFile);*/

	std::string fileName = "inst1.txt";

	std::ifstream dataFile(fileName);
	if (dataFile.fail()) {
		std::cout << fileName << " : file not found " << std::endl;
		return;
	}


	ConditionWithClasses cond(dataFile);

	if (!cond.Prepare()) {
		std::cout << "Initial Solution not feasible" << std::endl;
		exit(1);
	}

	std::cout << "conditions ready" << std::endl;

	std::shared_ptr<Solution> current(cond.getInitSolution()->clone());
	for (int i = 0; i < 10000; ++i) {
		current->LinKernighan(50);
		std::cout << "Iter: " << i << "  " << current->getOverLoad() << std::endl;
		//output << "Iter: " << i << "  " << current->getOverLoad() << std::endl;
	}
	//output.close();
}

void testGAwithClasses1();
void testGAwithClasses2();
void testGAwithClasses3();
void testAll();
void testRandomizedVsFullSearch();

void main() 
{
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); // enable file output
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtMemState _ms;
	//_CrtMemCheckpoint(&_ms);
	//
	//checkSolution("inst1.txt", "champ.txt");
	//_CrtMemDumpAllObjectsSince(&_ms)testGAwithLK();

	//testAll();
	testRandomizedVsFullSearch();
}
void testGAwithLK() {
	//std::ifstream dataFile("data_700_150.txt");
	std::ifstream dataFile("inst1.txt");

	Condition cond(dataFile);


	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		//MoveSwapStrategy,
		//RandomizedMoveSwapStrategy,
		LinKernighanStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	ga.setLog("output.txt");

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "LK.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
void ScalingTest() {
	std::ifstream dataFile("filesMigrationProblem2500.txt");
	
	ConditionWithHighLoad cond(dataFile);

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		RandomizedMoveSwapStrategy,
		//LinKernighanStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "_2500.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}

void ScalingTest2() {
	std::ifstream dataFile("filesMigrationProblem25000.txt");

	ConditionWithHighLoad cond(dataFile);

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		RandomizedMoveSwapStrategy,
		//LinKernighanStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "_25000.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}

void ScalingTest3() {
	std::ifstream dataFile("filesMigrationProblem10000.txt");

	ConditionWithHighLoad cond(dataFile);

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		RandomizedMoveSwapStrategy,
		//LinKernighanStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "_10000.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}

void testAll() {

	std::ofstream out("output-ALL.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

	//testGAwithMove();
	//testGAwithSwap();
	//testGA();
	testGAwithRandomSwapMove();
	testGAwithLK();
	
	//testGAWithViolations();

	//testGAwithClasses1();
	//testGAwithClasses2();
	//testGAwithClasses3();

	ScalingTest();
	ScalingTest3();
	ScalingTest2();

	RECOMMENDED_TIME *= 2;

	//testGAwithMove();
	//testGAwithSwap();
	//testGA();
	testGAwithRandomSwapMove();
	testGAwithLK();
	////testGAWithViolations();

	/*testGAwithClasses1();
	testGAwithClasses2();
	testGAwithClasses3();*/

	ScalingTest();
	ScalingTest3();
	ScalingTest2();

	std::cout.rdbuf(coutbuf); //reset to standard output again
}


void testGAwithClasses1() {

	//std::string fileName = "data_850_500.txt";
	std::string fileName = "inst1.txt";

	std::ifstream dataFile(fileName);
	if (dataFile.fail()) {
		std::cout << fileName << " : file not found " << std::endl;
		return;
	}


	ConditionWithClasses cond(dataFile);

	if (!cond.Prepare()) {
		std::cout << "Initial Solution not feasible" << std::endl;
		exit(1);
	}

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		//MoveSwapStrategy,
		RandomizedMoveSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;

	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "withClasses1.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
void testGAwithClasses2() {

	//std::string fileName = "data_850_500.txt";
	std::string fileName = "inst2.txt";

	std::ifstream dataFile(fileName);
	if (dataFile.fail()) {
		std::cout << fileName << " : file not found " << std::endl;
		return;
	}


	ConditionWithClasses cond(dataFile);

	if (!cond.Prepare()) {
		std::cout << "Initial Solution not feasible" << std::endl;
		exit(1);
	}

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		//MoveSwapStrategy,
		RandomizedMoveSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "withClasses2.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}
void testGAwithClasses3() {

	//std::string fileName = "data_850_500.txt";
	std::string fileName = "inst3.txt";

	std::ifstream dataFile(fileName);
	if (dataFile.fail()) {
		std::cout << fileName << " : file not found " << std::endl;
		return;
	}


	ConditionWithClasses cond(dataFile);

	if (!cond.Prepare()) {
		std::cout << "Initial Solution not feasible" << std::endl;
		exit(1);
	}

	BreedingStrategy<Solution> bstrategy(&cond, RECOMMENDED_POPULATION_SIZE);
	IterationNumberStopStrategy<Solution> sstrategy(RECOMMENDED_ITERATION_NUMBER);
	TimeStopStrategy<Solution> tsstrategy(RECOMMENDED_TIME);

	GeneticAlgorithm
		<Solution,
		strategies::Population,
		BreedingStrategy,
		//RandomizedSwapStrategy,
		//MoveSwapStrategy,
		RandomizedMoveSwapStrategy,
		//IterationNumberStopStrategy,
		TimeStopStrategy,
		SimpleCrossoverStrategy
		>
		ga(bstrategy, tsstrategy);

	std::clock_t start, finish;
	start = std::clock();
	ga.start();
	finish = std::clock();

	Solution * res = &*ga.getResult();
	res->calculateOverLoad();

	if (ga.getResult()->isFeasible()) {
		std::cout << "OK" << std::endl;
	}
	else {
		std::cout << "BAD" << std::endl;
	}
	std::cout << ga.getResult()->getOverLoad() << std::endl;
	std::cout << "time: " << (finish - start) / CLOCKS_PER_SEC << std::endl;


	if (ga.getResult()->isFeasible()) {

		std::stringstream outputStringStream;
		outputStringStream << ga.getResult()->getOverLoad() << "withClasses3.txt";
		std::cout << "Writing result to " << outputStringStream.str() << std::endl;

		std::ofstream result(outputStringStream.str());
		result << *ga.getResult() << std::endl;

	}
	else {
		std::cout << "Result is not feasible" << std::endl;
	}
}


void testRandomizedVsFullSearch() {
	testGAwithMove();
	testGAwithRandomMove();
	testGAwithSwap();
	testGAwithRandomSwap();
}