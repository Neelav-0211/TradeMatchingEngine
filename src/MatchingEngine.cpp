#include "MatchingEngine.hpp"

namespace tme {

MatchingEngine::MatchingEngine() = default;
MatchingEngine::~MatchingEngine() = default;

void MatchingEngine::processOrder(const Order& order) {
    auto orderBook = getOrCreateOrderBook(order.symbol);
    
    // Add the order to the order book
    orderBook->addOrder(order);
    
    // Try to match orders
    auto matches = orderBook->matchOrders();
    
    // Process matches (in a real system, this would notify traders, update positions, etc.)
    // Left as a placeholder for future implementation
}

bool MatchingEngine::cancelOrder(uint64_t orderId, const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = orderBooks_.find(symbol);
    if (it == orderBooks_.end()) {
        return false;  // Symbol not found
    }
    
    return it->second->cancelOrder(orderId);
}

std::shared_ptr<OrderBook> MatchingEngine::getOrderBook(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = orderBooks_.find(symbol);
    if (it == orderBooks_.end()) {
        return nullptr;  // Symbol not found
    }
    
    return it->second;
}

std::shared_ptr<OrderBook> MatchingEngine::getOrCreateOrderBook(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = orderBooks_.find(symbol);
    if (it == orderBooks_.end()) {
        // Create new order book for this symbol
        auto orderBook = std::make_shared<OrderBook>(symbol);
        orderBooks_[symbol] = orderBook;
        return orderBook;
    }
    
    return it->second;
}

} // namespace tme
