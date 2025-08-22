#pragma once

#include "OrderBook.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

namespace tme {

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
    
    // Cancel an existing order
    bool cancelOrder(uint64_t orderId, const std::string& symbol);
    
    // Get order book for a symbol
    std::shared_ptr<OrderBook> getOrderBook(const std::string& symbol);
    
private:
    // Map of symbol to order book
    std::unordered_map<std::string, std::shared_ptr<OrderBook>> orderBooks_;
    
    // Thread safety
    std::mutex mutex_;
    
    // Creates a new order book if it doesn't exist
    std::shared_ptr<OrderBook> getOrCreateOrderBook(const std::string& symbol);
};

} // namespace tme
