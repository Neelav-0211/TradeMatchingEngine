#pragma once

#include <chrono>
#include <string>
#include <cstdint>
#include <fstream>

namespace tme {
namespace perf {

struct BenchmarkResult {
    std::chrono::system_clock::time_point timestamp;
    uint64_t totalTimeMicroseconds;
    uint64_t numberOfSymbols;
    uint64_t numberOfOrders;
    double timePerOrderMicroseconds;
    std::string description;
};

class PerformanceRecorder {
public:
    static void recordResult(const BenchmarkResult& result, const std::string& outputFile);
    static void writeHeader(const std::string& outputFile);
    static bool fileExists(const std::string& filename);
    static std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp);
};

} // namespace perf
} // namespace tme
