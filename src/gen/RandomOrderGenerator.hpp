#pragma once

#include "../core/Order.hpp"
#include <random>
#include <string>
#include <atomic>

namespace tme {
namespace gen {

using namespace std;


// Generate a unique order ID
uint64_t generateOrderId();

// Create a random order
Order createRandomOrder(const string& symbol, mt19937& rng);

// Create a random order with custom price and quantity ranges
Order createRandomOrder(const string& symbol, mt19937& rng, 
                       double minPrice, double maxPrice, 
                       uint32_t minQuantity, uint32_t maxQuantity);

// Create a random order with specified side (buy/sell)
Order createRandomOrder(const string& symbol, mt19937& rng, Side side);

// Create a random order with custom parameters
Order createRandomOrder(const string& symbol, mt19937& rng,
                       double minPrice, double maxPrice,
                       uint32_t minQuantity, uint32_t maxQuantity,
                       Side side, OrderType type);

} // namespace gen
} // namespace tme
