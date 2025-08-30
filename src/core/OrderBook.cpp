#include "OrderBook.hpp"
#include <algorithm>
#include <cassert>

namespace tme {

using namespace std;

OrderBook::OrderBook(const string& symbol) : symbol_(symbol) {}

void OrderBook::addOrder(const Order& order) {
    unique_lock<shared_mutex> lock(mutex_);
    
    if (order.side == Side::BUY) {
        auto& priceLevel = buyOrders_[order.price];
        priceLevel.push_back(order);
        auto it = prev(priceLevel.end());
        orderLookup_[order.orderId] = make_pair(order.price, it);
    } else {
        auto& priceLevel = sellOrders_[order.price];
        priceLevel.push_back(order);
        auto it = prev(priceLevel.end());
        orderLookup_[order.orderId] = make_pair(order.price, it);
    }
}

bool OrderBook::cancelOrder(uint64_t orderId) {
    unique_lock<shared_mutex> lock(mutex_);
    
    auto lookup = orderLookup_.find(orderId);
    if (lookup == orderLookup_.end()) {
        return false;  // Order not found
    }
    
    const auto& price = lookup->second.first;
    const auto& orderIt = lookup->second.second;
    const auto& order = *orderIt;
    
    if (order.side == Side::BUY) {
        auto& priceLevel = buyOrders_[price];
        priceLevel.erase(orderIt);
        
        // Clean up empty price levels
        if (priceLevel.empty()) {
            buyOrders_.erase(price);
        }
    } else {
        auto& priceLevel = sellOrders_[price];
        priceLevel.erase(orderIt);
        
        // Clean up empty price levels
        if (priceLevel.empty()) {
            sellOrders_.erase(price);
        }
    }
    
    orderLookup_.erase(orderId);
    return true;
    
    

}

vector<pair<Order, Order>> OrderBook::matchOrders() {
    unique_lock<shared_mutex> lock(mutex_);
    vector<pair<Order, Order>> matches;
    
    // Keep matching as long as there are overlapping buy and sell orders
    while (!buyOrders_.empty() && !sellOrders_.empty()) {
        auto& bestBidLevel = buyOrders_.begin()->second;
        auto& bestAskLevel = sellOrders_.begin()->second;
        
        double bestBidPrice = buyOrders_.begin()->first;
        double bestAskPrice = sellOrders_.begin()->first;
        
        // No overlap, can't match more orders
        if (bestBidPrice < bestAskPrice) {
            break;
        }
        
        // Get the oldest orders at the best prices
        Order& buyOrder = bestBidLevel.front();
        Order& sellOrder = bestAskLevel.front();
        
        // Determine the matched quantity
        uint32_t matchedQuantity = min(buyOrder.quantity, sellOrder.quantity);
        
        // Create matched pair
        matches.emplace_back(buyOrder, sellOrder);
        
        // Update order quantities
        buyOrder.quantity -= matchedQuantity;
        sellOrder.quantity -= matchedQuantity;
        
        // Handle fully executed orders
        if (buyOrder.quantity == 0) {
            orderLookup_.erase(buyOrder.orderId);
            bestBidLevel.pop_front();
            
            if (bestBidLevel.empty()) {
                buyOrders_.erase(bestBidPrice);
            }
        }
        
        if (sellOrder.quantity == 0) {
            orderLookup_.erase(sellOrder.orderId);
            bestAskLevel.pop_front();
            
            if (bestAskLevel.empty()) {
                sellOrders_.erase(bestAskPrice);
            }
        }
    }
    
    return matches;
}

double OrderBook::getBestBid() const {
    shared_lock<shared_mutex> lock(mutex_);
    return buyOrders_.empty() ? 0.0 : buyOrders_.begin()->first;
}

double OrderBook::getBestAsk() const {
    shared_lock<shared_mutex> lock(mutex_);
    return sellOrders_.empty() ? 0.0 : sellOrders_.begin()->first;
}

uint32_t OrderBook::getVolumeAtPrice(Side side, double price) const {
    shared_lock<shared_mutex> lock(mutex_);
    
    if (side == Side::BUY) {
        auto it = buyOrders_.find(price);
        
        if (it == buyOrders_.end()) {
            return 0;
        }
        
        uint32_t volume = 0;
        for (const auto& order : it->second) {
            volume += order.quantity;
        }
        
        return volume;
    } else {
        auto it = sellOrders_.find(price);
        
        if (it == sellOrders_.end()) {
            return 0;
        }
        
        uint32_t volume = 0;
        for (const auto& order : it->second) {
            volume += order.quantity;
        }
        
        return volume;
    }
}

} // namespace tme
