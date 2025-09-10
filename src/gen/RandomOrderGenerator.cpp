#include "RandomOrderGenerator.hpp"
#include <chrono>

namespace tme {
namespace gen {

using namespace std;
using namespace std::chrono;

RandomOrderGenerator::RandomOrderGenerator(uint64_t seed, size_t num_tickers) : rng_(seed) {
    symbols_.reserve(num_tickers);
    for(size_t i = 0; i < num_tickers; ++i) {
        symbols_.push_back("SYM" + to_string(i));
    }
}

vector<Command> RandomOrderGenerator::generate(size_t total_commands) {
    vector<Command> cmds;
    cmds.reserve(total_commands);
    uniform_int_distribution<int> side_dist(0,1);
    uniform_int_distribution<int> px_base(9000, 11000); // cents
    uniform_int_distribution<int> sym_dist(0, (int)symbols_.size()-1);
    uniform_int_distribution<int> qty_dist(1, 1000);

    for(size_t i = 0; i < total_commands; ++i) {
        Order o;
        o.orderId = next_order_id_++;
        o.side = side_dist(rng_) ? Side::BUY : Side::SELL;
        o.price = px_base(rng_) + (o.side == Side::BUY ? - (rng_()%100) : + (rng_()%100));
        o.symbol = symbols_[sym_dist(rng_)];
        o.quantity = qty_dist(rng_);
        o.timestamp = steady_clock::now();
        o.type = OrderType::MARKET;
        cmds.emplace_back(NewOrder{o});
   }
   return cmds;
}

} // namespace gen
} // namespace tme 
