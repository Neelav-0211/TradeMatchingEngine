#include "gtest/gtest.h"
#include "../src/core/OrderBook.hpp"
#include "../src/core/MatchingEngine.hpp"

using namespace tme;

TEST(OrderBookTest, AddAndCancelOrder) {
    OrderBook orderBook("AAPL");
    
    // Create a buy order
    Order buyOrder;
    buyOrder.orderId = 1;
    buyOrder.symbol = "AAPL";
    buyOrder.price = 100.0;
    buyOrder.quantity = 10;
    buyOrder.side = Side::BUY;
    buyOrder.type = OrderType::LIMIT;
    
    // Add the order
    orderBook.addOrder(buyOrder);
    
    // Check that the order was added correctly
    EXPECT_EQ(orderBook.getBestBid(), 100.0);
    EXPECT_EQ(orderBook.getVolumeAtPrice(Side::BUY, 100.0), 10);
    
    // Cancel the order
    bool success = orderBook.cancelOrder(1);
    EXPECT_TRUE(success);
    
    // Check that the order was cancelled
    EXPECT_EQ(orderBook.getVolumeAtPrice(Side::BUY, 100.0), 0);
    EXPECT_EQ(orderBook.getBestBid(), 0.0);  // No more bids
}

TEST(OrderBookTest, MatchOrders) {
    OrderBook orderBook("AAPL");
    
    // Create a buy order
    Order buyOrder;
    buyOrder.orderId = 1;
    buyOrder.symbol = "AAPL";
    buyOrder.price = 100.0;
    buyOrder.quantity = 10;
    buyOrder.side = Side::BUY;
    buyOrder.type = OrderType::LIMIT;
    
    // Create a matching sell order
    Order sellOrder;
    sellOrder.orderId = 2;
    sellOrder.symbol = "AAPL";
    sellOrder.price = 100.0;
    sellOrder.quantity = 5;
    sellOrder.side = Side::SELL;
    sellOrder.type = OrderType::LIMIT;
    
    // Add the orders
    orderBook.addOrder(buyOrder);
    orderBook.addOrder(sellOrder);
    
    // Match the orders
    auto matches = orderBook.matchOrders();
    
    // Check that the orders were matched
    EXPECT_EQ(matches.size(), 1);
    EXPECT_EQ(matches[0].first.orderId, 1);
    EXPECT_EQ(matches[0].second.orderId, 2);
    
    // Check that the buy order still has 5 shares left
    EXPECT_EQ(orderBook.getVolumeAtPrice(Side::BUY, 100.0), 5);
    
    // Check that the sell order is fully matched and removed
    EXPECT_EQ(orderBook.getVolumeAtPrice(Side::SELL, 100.0), 0);
}

TEST(MatchingEngineTest, ProcessOrders) {
    MatchingEngine engine;
    
    // Create a buy order
    Order buyOrder;
    buyOrder.orderId = 1;
    buyOrder.symbol = "AAPL";
    buyOrder.price = 100.0;
    buyOrder.quantity = 10;
    buyOrder.side = Side::BUY;
    buyOrder.type = OrderType::LIMIT;
    
    // Process the order
    engine.processOrder(buyOrder);
    
    // Get the order book
    auto orderBook = engine.getOrderBook("AAPL");
    ASSERT_TRUE(orderBook != nullptr);
    
    // Check that the order was added correctly
    EXPECT_EQ(orderBook->getBestBid(), 100.0);
    EXPECT_EQ(orderBook->getVolumeAtPrice(Side::BUY, 100.0), 10);
}
