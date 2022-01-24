#pragma once
#include <random>

class Random {
public:
	Random();
	~Random();
	int randInt(int min, int max);

private:
	std::random_device mSeedGen;
	std::mt19937 mEngine;
};