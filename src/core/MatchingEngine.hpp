#pragma once

#include "OrderBook.hpp"
#include "Command.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <vector>

namespace tme {

using namespace std;

/**
 * The main matching engine class that processes orders and maintains
 * order books for different symbols.
 */
class MatchingEngine {
public:
    MatchingEngine();
    ~MatchingEngine();
    
    // Process a new order
    void processOrder(const Order& order);
    
    // Process a batch of commands efficiently
    void processBatch(const vector<Command>& commands);
    
    // Cancel an existing order
    bool cancelOrder(uint64_t orderId, const string& symbol);
    
    // Get order book for a symbol
    shared_ptr<OrderBook> getOrderBook(const string& symbol);
    
private:
    // Map of symbol to order book
    unordered_map<string, shared_ptr<OrderBook>> orderBooks_;
    
    // Thread safety
    mutex mutex_;
    
    // Creates a new order book if it doesn't exist
    shared_ptr<OrderBook> getOrCreateOrderBook(const string& symbol);
};

} // namespace tme
