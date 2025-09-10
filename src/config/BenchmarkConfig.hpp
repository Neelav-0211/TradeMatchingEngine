#pragma once

#include <string>
#include <cstdint>

namespace tme {
namespace config {

// Benchmark configuration constants
class BenchmarkConfig {
public:
    // Test parameters
    static constexpr uint64_t NUM_ORDERS = 10000000;
    static constexpr uint64_t NUM_SYMBOLS = 100;
    static constexpr const char* OUTPUT_FILE = "../benchmark_results.csv";
    
    // Test description
    static const std::string TEST_DESCRIPTION;
    
    // Output file headers
    static const std::string CSV_HEADER;
};

} // namespace config
} // namespace tme
