#pragma once
#include <random>

// simple stock market simulator
// of course uniform distribution is for simplicity

class MarketSimulator
{
	const int gMaxStocks = 10000;
	const double gStartPrice = 100;
	const int MarketDeviation = 100;

	std::random_device dev;
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> distStock; 
	std::uniform_int_distribution<std::mt19937::result_type> distPrice; 

public:

	MarketSimulator() : rng(dev()), distStock(0, gMaxStocks), distPrice(0, MarketDeviation)
	{

	}

	int RandomStock()
	{
		return distStock(rng);
	}

	double RandomPrice()
	{
		auto rnd = distPrice(rng);
		return gStartPrice * (1. + 0.01 * ((double)MarketDeviation / 2 - rnd));
	}
};

