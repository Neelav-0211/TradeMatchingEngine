#pragma once

#include "Order.hpp"
#include <unordered_map>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace tme {

/**
 * OrderBook maintains a list of buy and sell orders for a specific instrument.
 * It's optimized for fast insertion, deletion, and matching of orders.
 */
class OrderBook {
public:
    explicit OrderBook(const std::string& symbol);
    
    // Add a new order to the book
    void addOrder(const Order& order);
    
    // Cancel an existing order
    bool cancelOrder(uint64_t orderId);
    
    // Match orders and execute trades
    std::vector<std::pair<Order, Order>> matchOrders();
    
    // Get best bid/ask prices
    double getBestBid() const;
    double getBestAsk() const;
    
    // Get total volume at a price level
    uint32_t getVolumeAtPrice(Side side, double price) const;
    
private:
    std::string symbol_;
    
    // Price-time priority queues for buy and sell orders
    // Using map for price levels and list for time priority
    std::map<double, std::list<Order>, std::greater<>> buyOrders_;  // Higher prices first
    std::map<double, std::list<Order>> sellOrders_;                 // Lower prices first
    
    // Fast lookup by order ID
    std::unordered_map<uint64_t, std::pair<double, std::list<Order>::iterator>> orderLookup_;
    
    // Thread safety
    mutable std::shared_mutex mutex_;
};

} // namespace tme
