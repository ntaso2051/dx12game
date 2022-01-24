#include "Random.h"

Random::Random() : mEngine(mSeedGen()) {}

Random::~Random() {}

int Random::randInt(int min, int max) {
	std::uniform_int_distribution<> randgen(min, max);
	return randgen(mEngine);
}