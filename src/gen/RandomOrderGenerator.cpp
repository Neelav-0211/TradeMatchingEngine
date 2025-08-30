#include "RandomOrderGenerator.hpp"
#include <chrono>
#include <cmath>

namespace tme {
namespace gen {

using namespace std;
using namespace std::chrono;

// Generate a unique order ID
uint64_t generateOrderId() {
    static atomic<uint64_t> nextOrderId{1};
    return nextOrderId.fetch_add(1, memory_order_relaxed);
}

// Create a random order with default parameters
Order createRandomOrder(const string& symbol, mt19937& rng) {
    return createRandomOrder(symbol, rng, 90.0, 110.0, 1, 100);
}

// Create a random order with custom price and quantity ranges
Order createRandomOrder(const string& symbol, mt19937& rng, 
                       double minPrice, double maxPrice, 
                       uint32_t minQuantity, uint32_t maxQuantity) {
    // 50% chance of buy or sell
    uniform_int_distribution<> sideDist(0, 1);
    Side side = sideDist(rng) == 0 ? Side::BUY : Side::SELL;
    
    return createRandomOrder(symbol, rng, minPrice, maxPrice, 
                           minQuantity, maxQuantity, side, OrderType::LIMIT);
}

// Create a random order with specified side
Order createRandomOrder(const string& symbol, mt19937& rng, Side side) {
    return createRandomOrder(symbol, rng, 90.0, 110.0, 1, 100, side, OrderType::LIMIT);
}

// Create a random order with all custom parameters
Order createRandomOrder(const string& symbol, mt19937& rng,
                       double minPrice, double maxPrice,
                       uint32_t minQuantity, uint32_t maxQuantity,
                       Side side, OrderType type) {
    // Price distribution
    uniform_real_distribution<> priceDist(minPrice, maxPrice);
    
    // Quantity distribution
    uniform_int_distribution<uint32_t> quantityDist(minQuantity, maxQuantity);
    
    Order order;
    order.orderId = generateOrderId();
    order.symbol = symbol;
    order.price = round(priceDist(rng) * 100) / 100;  // Round to 2 decimal places
    order.quantity = quantityDist(rng);
    order.side = side;
    order.type = type;
    order.timestamp = steady_clock::now();
    
    return order;
}

} // namespace gen
} // namespace tme
