#include "core/MatchingEngine.hpp"
#include "gen/RandomOrderGenerator.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>

using namespace tme;
using namespace tme::gen;
using namespace std::chrono;
using namespace std;

// Print order book status
void printOrderBookStatus(const shared_ptr<OrderBook>& orderBook) {
    double bestBid = orderBook->getBestBid();
    double bestAsk = orderBook->getBestAsk();
    
    cout << "Best Bid: " << fixed << setprecision(2) << bestBid;
    cout << " (" << orderBook->getVolumeAtPrice(Side::BUY, bestBid) << ")";
    
    cout << " | Best Ask: " << bestAsk;
    cout << " (" << orderBook->getVolumeAtPrice(Side::SELL, bestAsk) << ")" << endl;
    
    double spread = bestAsk - bestBid;
    if (bestBid > 0 && bestAsk > 0) {
        cout << "Spread: " << spread << " (" << (spread / bestBid * 100) << "%)" << endl;
    }
}

// Benchmark adding orders
void benchmarkAddOrders(MatchingEngine& engine, const string& symbol, int numOrders) {
    random_device rd;
    mt19937 rng(rd());
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < numOrders; ++i) {
        auto order = createRandomOrder(symbol, rng);
        engine.processOrder(order);
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Added " << numOrders << " orders in " << duration.count() << " microseconds" << endl;
    cout << "Average time per order: " << (duration.count() / numOrders) << " microseconds" << endl;
}

int main() {
    MatchingEngine engine;
    const string symbol = "AAPL";
    
    cout << "Trade Matching Engine Demo" << endl;
    cout << "-------------------------" << endl;
    
    // Benchmark with 10,000 orders
    benchmarkAddOrders(engine, symbol, 10000);
    
    // Get and print order book status
    auto orderBook = engine.getOrderBook(symbol);
    if (orderBook) {
        printOrderBookStatus(orderBook);
    }
    
    return 0;
}
