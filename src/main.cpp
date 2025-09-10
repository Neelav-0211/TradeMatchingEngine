#include "core/MatchingEngine.hpp"
#include "gen/RandomOrderGenerator.hpp"
#include "config/BenchmarkConfig.hpp"
#include "perf/PerformanceRecorder.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>

using namespace tme;
using namespace tme::gen;
using namespace tme::config;
using namespace tme::perf;
using namespace std::chrono;
using namespace std;

// Print order book status
void printOrderBookStatus(const shared_ptr<OrderBook>& orderBook) {
    uint32_t bestBid = orderBook->getBestBid();
    uint32_t bestAsk = orderBook->getBestAsk();
    
    cout << "Best Bid: " << fixed << setprecision(2) << bestBid;
    cout << " (" << orderBook->getVolumeAtPrice(Side::BUY, bestBid) << ")";
    
    cout << " | Best Ask: " << bestAsk;
    cout << " (" << orderBook->getVolumeAtPrice(Side::SELL, bestAsk) << ")" << endl;
    
    uint32_t spread = bestAsk - bestBid;
    if (bestBid > 0 && bestAsk > 0) {
        cout << "Spread: " << spread << " (" << (static_cast<double>(spread) / bestBid * 100) << "%)" << endl;
    }
}

// Benchmark orders and record performance metrics
BenchmarkResult benchmarkAddOrders(MatchingEngine& engine, uint64_t numOrders, uint64_t num_symbols) {
    random_device rd;
    RandomOrderGenerator generator(rd(), num_symbols); 
    
    cout << "Generating " << numOrders << " orders..." << endl;
    auto genStart = high_resolution_clock::now();
    auto commands = generator.generate(numOrders);
    auto genEnd = high_resolution_clock::now();
    auto genDuration = duration_cast<microseconds>(genEnd - genStart);
    
    cout << "Order generation completed in " << genDuration.count() << " microseconds" << endl;
    cout << "Processing orders in parallel batches grouped by symbol..." << endl;
    
    auto start = high_resolution_clock::now();
    auto timestamp = system_clock::now(); // Record timestamp for the benchmark
    engine.processBatch(commands);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    double avgTimePerOrder = static_cast<double>(duration.count()) / numOrders;
    
    cout << "Processed " << numOrders << " orders in " << duration.count() << " microseconds" << endl;
    cout << "Average time per order: " << fixed << setprecision(3) << avgTimePerOrder << " microseconds" << endl;
    
    // Create and return benchmark result
    BenchmarkResult result;
    result.timestamp = timestamp;
    result.totalTimeMicroseconds = duration.count();
    result.numberOfSymbols = num_symbols;
    result.numberOfOrders = numOrders;
    result.timePerOrderMicroseconds = avgTimePerOrder;
    result.description = BenchmarkConfig::TEST_DESCRIPTION;
    
    return result;
}

int main() {
    // Use parallel matching engine with configured number of threads
    MatchingEngine engine(BenchmarkConfig::NUM_THREADS);
    
    cout << "Trade Matching Engine Demo (Parallel)" << endl;
    cout << "Using " << BenchmarkConfig::NUM_THREADS << " worker threads" << endl;
    cout << "-------------------------------------" << endl;

    // Use constants from configuration
    const uint64_t num_orders = BenchmarkConfig::NUM_ORDERS;
    const uint64_t num_symbols = BenchmarkConfig::NUM_SYMBOLS;
    
    // Run benchmark and get results
    BenchmarkResult result = benchmarkAddOrders(engine, num_orders, num_symbols);
    
    // Record results to output file
    PerformanceRecorder::recordResult(result, BenchmarkConfig::OUTPUT_FILE);
    
    // Get and print order book status for the first symbol
    auto orderBook = engine.getOrderBook("SYM0");
    if (orderBook) {
        printOrderBookStatus(orderBook);
    }
    
    return 0;
}
