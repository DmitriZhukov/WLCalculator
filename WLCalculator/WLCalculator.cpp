// WLCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <set>
#include <random>
#include <thread>

#include "quote.h"
#include "Engine.h"
#include "MarketSimulator.h"
#include "Listener.h"

void QuotesLoop(IEngine *pEngine, MarketSimulator &ms, std::atomic_bool& cancellation_token)
{
    while (!cancellation_token)
    {
        int stock_id = ms.RandomStock();
        double price = ms.RandomPrice();

        pEngine->OnQuote(stock_id, price);
    }
}

int main()
{
    std::cout << "Staring Simulation. Press any key to stop.\n";

    std::atomic_bool cancellation_token = false;

    Engine engine;
    MarketSimulator ms;
    Listener lsG;
    Listener lsL;

    engine.GainersCallbackPtr = &lsG;
    engine.LosersCallbackPtr = &lsL;

    std::thread t(QuotesLoop, &engine, std::ref(ms), std::ref(cancellation_token));

    std::cin.get();

    cancellation_token = true; 

    t.join();
}




