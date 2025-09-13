#include "BenchmarkConfig.hpp"

namespace tme {
namespace config {

// Test description - modify this for each test run
const std::string BenchmarkConfig::TEST_DESCRIPTION = "Notifying worker threads as soon as task created with parallel execution(16 threads).";

// CSV header for output file
const std::string BenchmarkConfig::CSV_HEADER = "Timestamp,Total_Time_Microseconds,Number_of_Symbols,Number_of_Orders,Time_per_Order_Microseconds,Description";

} // namespace config
} // namespace tme
