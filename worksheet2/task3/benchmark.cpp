#include "bump_up.hpp"
#include "benchmark.hpp"
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>

struct BenchmarkResult {
    double mean_duration;
    double std_deviation;
    size_t num_runs;
};

BenchmarkResult run_benchmark(const std::string& name, 
                            std::function<void(bump_allocator&)> func, 
                            bump_allocator& allocator,
                            size_t num_runs = 10) {
    std::vector<double> durations;
    durations.reserve(num_runs);

    for (size_t i = 0; i < num_runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func(allocator);
        auto end = std::chrono::high_resolution_clock::now();
        
        double duration = std::chrono::duration<double, std::milli>(end - start).count();
        durations.push_back(duration);
        
        allocator.dealloc(); // Reset after each run
    }

    // Calculate mean
    double sum = 0.0;
    for (double d : durations) {
        sum += d;
    }
    double mean = sum / num_runs;

    // Calculate standard deviation
    double sq_sum = 0.0;
    for (double d : durations) {
        sq_sum += (d - mean) * (d - mean);
    }
    double std_dev = std::sqrt(sq_sum / (num_runs - 1));

    return {mean, std_dev, num_runs};
}

void print_results(const std::string& name, const BenchmarkResult& result) {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Runs: " << result.num_runs << std::endl;
    std::cout << "Mean time: " << result.mean_duration << " ms" << std::endl;
    std::cout << "Std Dev: ± " << result.std_deviation << " ms" << std::endl;
}

void small_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 1000; ++i) {
        allocator.alloc<int>(1);
    }
}

void large_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 100; ++i) {
        allocator.alloc<int>(100);
    }
}

void mixed_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 500; ++i) {
        allocator.alloc<int>(i % 10 + 1);
    }
}

int main() {
    const size_t NUM_RUNS = 10;
    bump_allocator allocator(1024 * 1024); // 1 MB

    std::cout << "Running benchmarks with " << NUM_RUNS << " iterations each...\n" << std::endl;

    auto small_result = run_benchmark("Small Allocations", small_allocations, 
                                    allocator, NUM_RUNS);
    print_results("Small Allocations", small_result);

    auto large_result = run_benchmark("Large Allocations", large_allocations, 
                                    allocator, NUM_RUNS);
    print_results("Large Allocations", large_result);

    auto mixed_result = run_benchmark("Mixed Allocations", mixed_allocations, 
                                    allocator, NUM_RUNS);
    print_results("Mixed Allocations", mixed_result);

    return 0;
}