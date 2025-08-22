#include "MatchingEngine.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>

using namespace tme;
using namespace std::chrono;

// Generate a unique order ID
uint64_t generateOrderId() {
    static std::atomic<uint64_t> nextOrderId{1};
    return nextOrderId.fetch_add(1, std::memory_order_relaxed);
}

// Create a random order
Order createRandomOrder(const std::string& symbol, std::mt19937& rng) {
    // Price between 90.0 and 110.0
    std::uniform_real_distribution<> priceDist(90.0, 110.0);
    
    // Quantity between 1 and 100
    std::uniform_int_distribution<> quantityDist(1, 100);
    
    // 50% chance of buy or sell
    std::uniform_int_distribution<> sideDist(0, 1);
    
    Order order;
    order.orderId = generateOrderId();
    order.symbol = symbol;
    order.price = std::round(priceDist(rng) * 100) / 100;  // Round to 2 decimal places
    order.quantity = quantityDist(rng);
    order.side = sideDist(rng) == 0 ? Side::BUY : Side::SELL;
    order.type = OrderType::LIMIT;
    order.timestamp = steady_clock::now();
    
    return order;
}

// Print order book status
void printOrderBookStatus(const std::shared_ptr<OrderBook>& orderBook) {
    double bestBid = orderBook->getBestBid();
    double bestAsk = orderBook->getBestAsk();
    
    std::cout << "Best Bid: " << std::fixed << std::setprecision(2) << bestBid;
    std::cout << " (" << orderBook->getVolumeAtPrice(Side::BUY, bestBid) << ")";
    
    std::cout << " | Best Ask: " << bestAsk;
    std::cout << " (" << orderBook->getVolumeAtPrice(Side::SELL, bestAsk) << ")" << std::endl;
    
    double spread = bestAsk - bestBid;
    if (bestBid > 0 && bestAsk > 0) {
        std::cout << "Spread: " << spread << " (" << (spread / bestBid * 100) << "%)" << std::endl;
    }
}

// Benchmark adding orders
void benchmarkAddOrders(MatchingEngine& engine, const std::string& symbol, int numOrders) {
    std::random_device rd;
    std::mt19937 rng(rd());
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < numOrders; ++i) {
        auto order = createRandomOrder(symbol, rng);
        engine.processOrder(order);
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    std::cout << "Added " << numOrders << " orders in " << duration.count() << " microseconds" << std::endl;
    std::cout << "Average time per order: " << (duration.count() / numOrders) << " microseconds" << std::endl;
}

int main() {
    MatchingEngine engine;
    const std::string symbol = "AAPL";
    
    std::cout << "Trade Matching Engine Demo" << std::endl;
    std::cout << "-------------------------" << std::endl;
    
    // Benchmark with 10,000 orders
    benchmarkAddOrders(engine, symbol, 10000);
    
    // Get and print order book status
    auto orderBook = engine.getOrderBook(symbol);
    if (orderBook) {
        printOrderBookStatus(orderBook);
    }
    
    return 0;
}
