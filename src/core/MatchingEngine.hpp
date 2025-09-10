#pragma once

#include "OrderBook.hpp"
#include "Command.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <atomic>

namespace tme {

using namespace std;

/**
 * The main matching engine class that processes orders and maintains
 * order books for different symbols with parallel processing capabilities.
 */
class MatchingEngine {
public:
    // Constructor with configurable thread count
    explicit MatchingEngine(size_t numThreads = 4);
    ~MatchingEngine();
    
    // Process a new order
    void processOrder(const Order& order);
    
    // Process a batch of commands efficiently with parallel processing
    void processBatch(const vector<Command>& commands);
    
    // Cancel an existing order
    bool cancelOrder(uint64_t orderId, const string& symbol);
    
    // Get order book for a symbol
    shared_ptr<OrderBook> getOrderBook(const string& symbol);
    
private:
    // Task structure for thread pool
    struct Task {
        string symbol;
        vector<Order> orders;
    };
    
    // Map of symbol to order book
    unordered_map<string, shared_ptr<OrderBook>> orderBooks_;
    
    // Thread pool management
    vector<thread> workers_;
    queue<Task> tasks_;
    mutex taskMutex_;
    condition_variable taskCondition_;
    atomic<bool> shutdown_;
    
    // Thread safety for orderBooks map
    mutex orderBooksMutex_;
    
    // Thread pool worker function
    void workerThread();
    
    // Process a single symbol's orders
    void processSymbolOrders(const string& symbol, const vector<Order>& orders);
    
    // Creates a new order book if it doesn't exist
    shared_ptr<OrderBook> getOrCreateOrderBook(const string& symbol);
    
    // Initialize thread pool
    void initializeThreadPool(size_t numThreads);
    
    // Shutdown thread pool
    void shutdownThreadPool();
};

} // namespace tme
