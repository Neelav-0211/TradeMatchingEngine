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

using namespace std;

/**
 * OrderBook maintains a list of buy and sell orders for a specific instrument.
 * It's optimized for fast insertion, deletion, and matching of orders.
 */
class OrderBook {
public:
    explicit OrderBook(const string& symbol);
    
    // Add a new order to the book
    void addOrder(const Order& order);
    
    // Add multiple orders efficiently in bulk
    void addOrdersBatch(const vector<Order>& orders);
    
    // Cancel an existing order
    bool cancelOrder(uint64_t orderId);
    
    // Match orders and execute trades
    vector<pair<Order, Order>> matchOrders();
    
    // Get best bid/ask prices
    uint32_t getBestBid() const;
    uint32_t getBestAsk() const;
    
    // Get total volume at a price level
    uint32_t getVolumeAtPrice(Side side, uint32_t price) const;
    
private:
    string symbol_;
    
    // Price-time priority queues for buy and sell orders
    // Using map for price levels and list for time priority
    map<uint32_t, list<Order>, greater<>> buyOrders_;  // Higher prices first
    map<uint32_t, list<Order>> sellOrders_;            // Lower prices first
    
    // Fast lookup by order ID
    unordered_map<uint64_t, pair<uint32_t, list<Order>::iterator>> orderLookup_;
    
    // Thread safety
    mutable shared_mutex mutex_;
};

} // namespace tme
