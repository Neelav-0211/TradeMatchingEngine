#pragma once

#include "../core/Order.hpp"
#include "../core/Command.hpp"
#include <random>
#include <string>

namespace tme {
namespace gen {

using namespace tme;
using namespace std;

class RandomOrderGenerator {
public: 
    RandomOrderGenerator(uint64_t seed, size_t num_tickers);
    
    vector<Command> generate(size_t total_commands);

private:
    mt19937_64 rng_;
    vector<string> symbols_;
    uint64_t next_order_id_{1};
};

} // namespace gen
} // namespace tme
