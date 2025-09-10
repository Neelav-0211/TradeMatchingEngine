#include "MatchingEngine.hpp"
#include <unordered_map>
#include <algorithm>

namespace tme
{

    using namespace std;

    MatchingEngine::MatchingEngine(size_t numThreads) : shutdown_(false) {
        initializeThreadPool(numThreads);
    }

    MatchingEngine::~MatchingEngine() {
        shutdownThreadPool();
    }

    void MatchingEngine::initializeThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back(&MatchingEngine::workerThread, this);
        }
    }

    void MatchingEngine::shutdownThreadPool() {
        {
            lock_guard<mutex> lock(taskMutex_);
            shutdown_ = true;
        }
        
        taskCondition_.notify_all();
        
        for (thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void MatchingEngine::workerThread() {
        while (true) {
            Task task;
            
            {
                unique_lock<mutex> lock(taskMutex_);
                taskCondition_.wait(lock, [this] { return !tasks_.empty() || shutdown_; });
                
                if (shutdown_ && tasks_.empty()) {
                    break;
                }
                
                if (!tasks_.empty()) {
                    task = move(tasks_.front());
                    tasks_.pop();
                }
            }
            
            if (!task.symbol.empty()) {
                processSymbolOrders(task.symbol, task.orders);
            }
        }
    }

    void MatchingEngine::processOrder(const Order &order)
    {
        auto orderBook = getOrCreateOrderBook(order.symbol);

        // Add the order to the order book
        orderBook->addOrder(order);

        // Try to match orders
        auto matches = orderBook->matchOrders();

        // Process matches (in a real system, this would notify traders, update positions, etc.)
        // Left as a placeholder for future implementation
    }

    void MatchingEngine::processBatch(const vector<Command> &commands)
    {
        // Group commands by symbol for better performance
        unordered_map<string, vector<Order>> symbolGroups;

        // Group orders by symbol
        for (const Command &cmd : commands)
        {
            if (holds_alternative<NewOrder>(cmd))
            {
                const NewOrder newOrder = get<NewOrder>(cmd);
                symbolGroups[newOrder.order.symbol].push_back(newOrder.order);
            }
        }
        
        // Submit tasks to thread pool for parallel processing
        {
            lock_guard<mutex> lock(taskMutex_);
            for (const auto &[symbol, orders] : symbolGroups) {
                tasks_.push({symbol, orders});
            }
        }
        
        taskCondition_.notify_all();
        
        // Wait for all tasks to complete using a counter-based approach
        size_t totalTasks = symbolGroups.size();
        size_t processedTasks = 0;
        
        while (processedTasks < totalTasks) {
            {
                lock_guard<mutex> lock(taskMutex_);
                size_t remainingTasks = tasks_.size();
                processedTasks = totalTasks - remainingTasks;
            }
            
            if (processedTasks < totalTasks) {
                this_thread::sleep_for(chrono::microseconds(10));
            }
        }
        
        // Additional small delay to ensure all processing is complete
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    void MatchingEngine::processSymbolOrders(const string& symbol, const vector<Order>& orders) {
        auto orderBook = getOrCreateOrderBook(symbol);
        
        const size_t MATCH_BATCH_SIZE = 100;
        
        // Process orders in chunks using bulk insertion
        for (size_t i = 0; i < orders.size(); i += MATCH_BATCH_SIZE) {
            size_t endIdx = min(i + MATCH_BATCH_SIZE, orders.size());
            vector<Order> batch(orders.begin() + i, orders.begin() + endIdx);
            
            // Add batch of orders
            orderBook->addOrdersBatch(batch);
            
            // Match after each batch
            auto matches = orderBook->matchOrders();
            // Process matches (in a real system, this would notify traders, update positions, etc.)
            // Left as a placeholder for future implementation
        }
    }

    bool MatchingEngine::cancelOrder(uint64_t orderId, const string &symbol)
    {
        shared_ptr<OrderBook> orderBook;

        // Only lock while accessing the map
        {
            lock_guard<mutex> lock(orderBooksMutex_);
            auto it = orderBooks_.find(symbol);
            if (it == orderBooks_.end())
            {
                return false; // Symbol not found
            }
            orderBook = it->second;
        }

        // Now use the thread-safe orderBook outside the lock
        return orderBook->cancelOrder(orderId);
    }

    shared_ptr<OrderBook> MatchingEngine::getOrderBook(const string &symbol)
    {
        lock_guard<mutex> lock(orderBooksMutex_);

        auto it = orderBooks_.find(symbol);
        if (it == orderBooks_.end())
        {
            return nullptr; // Symbol not found
        }

        return it->second;
    }

    shared_ptr<OrderBook> MatchingEngine::getOrCreateOrderBook(const string &symbol)
    {
        lock_guard<mutex> lock(orderBooksMutex_);

        auto it = orderBooks_.find(symbol);
        if (it == orderBooks_.end())
        {
            // Create new order book for this symbol
            auto orderBook = make_shared<OrderBook>(symbol);
            orderBooks_[symbol] = orderBook;
            return orderBook;
        }

        return it->second;
    }

} // namespace tme
