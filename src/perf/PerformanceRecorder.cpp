#include "PerformanceRecorder.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace tme {
namespace perf {

void PerformanceRecorder::recordResult(const BenchmarkResult& result, const std::string& outputFile) {
    bool fileExisted = fileExists(outputFile);
    
    std::ofstream file(outputFile, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFile << std::endl;
        return;
    }
    
    // Write header if file didn't exist
    if (!fileExisted) {
        file << "Timestamp,Total_Time_Microseconds,Number_of_Symbols,Number_of_Orders,Time_per_Order_Microseconds,Description\n";
    }
    
    // Write the benchmark result
    file << formatTimestamp(result.timestamp) << ","
         << result.totalTimeMicroseconds << ","
         << result.numberOfSymbols << ","
         << result.numberOfOrders << ","
         << std::fixed << std::setprecision(6) << result.timePerOrderMicroseconds << ","
         << "\"" << result.description << "\"\n";
    
    file.close();
    
    std::cout << "Benchmark result recorded to " << outputFile << std::endl;
}

void PerformanceRecorder::writeHeader(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (file.is_open()) {
        file << "Timestamp,Total_Time_Microseconds,Number_of_Symbols,Number_of_Orders,Time_per_Order_Microseconds,Description\n";
        file.close();
    }
}

bool PerformanceRecorder::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

std::string PerformanceRecorder::formatTimestamp(const std::chrono::system_clock::time_point& timestamp) {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        timestamp.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

} // namespace perf
} // namespace tme
