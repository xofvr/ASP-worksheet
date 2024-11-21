#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <functional>
#include <iostream>

class Benchmark {
public:
    template <typename Func, typename... Args>
    static void measure(const std::string& name, Func func, Args&&... args) {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = high_resolution_clock::now();
        duration<double, std::milli> ms_double = end - start;
        std::cout << name << ": " << ms_double.count() << "ms\n";
    }
};

#endif 