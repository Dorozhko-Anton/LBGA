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

	GlobalRNG() : mt(std::random_device()()) {
	
		std::uniform_int_distribution<int> dist(0, 100);

		for (int i = 1; i < 16; i++) {
			std::cout << dist(mt) << " ";
		}
		std::cout << std::endl;
	}

	
};

#endif