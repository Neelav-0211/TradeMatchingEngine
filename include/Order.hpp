#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace tme {

enum class OrderType {
    LIMIT,
    MARKET,
    STOP,
    STOP_LIMIT
};

enum class Side {
    BUY,
    SELL
};

struct Order {
    uint64_t orderId;
    std::string symbol;
    double price;
    uint32_t quantity;
    Side side;
    OrderType type;
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    
    // For efficient comparison in containers
    bool operator<(const Order& other) const {
        // For buy orders, higher prices come first
        if (side == Side::BUY) {
            return price > other.price;
        }
        // For sell orders, lower prices come first
        return price < other.price;
    }
};

} // namespace tme
