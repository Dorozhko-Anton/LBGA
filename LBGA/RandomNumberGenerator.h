#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H
#pragma once

#include <random>
#include <iostream>

class GlobalRNG {
public:
	static GlobalRNG& getInstance()
	{
		static GlobalRNG instance;

		return instance;
	}
	std::mt19937& getEngine() {

		return mt;
	}
private:
	
	std::mt19937 mt;

	GlobalRNG() : mt(std::random_device()()) {}
};

#endif