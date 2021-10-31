#include <iostream>
#include <chrono>
#include "DVector.hpp"
#include "Constants.hpp"

inline void std_benchmark_depend(DVector &vec1, DVector &vec2, DVector &vec3) {
    auto clocks_total = 0;
    register int raw_v1 asm("%ah") = 0;
    register int raw_v2 asm("%al") = 0;
    register double res_1 asm("%dx") = 0;
    register double res_2 asm("%dl") = 0;

    for (std::size_t i = 0; i < vec1.Size(); ++i) {
        raw_v1 = vec1.GetRawBlock(i);
        raw_v2 = vec2.GetRawBlock(i);
        auto r1 = rdtsc();
        res_1 = static_cast<double>(raw_v1) + static_cast<double>(raw_v2);
        res_2 = static_cast<double>(res_1) + static_cast<double>(raw_v2);
        auto r2 = rdtsc();
        clocks_total += approx_rdtsc(r2 - r1);
    }
    std::cout << "DEPENDENCE OPERATIONS:\n";
    std::cout << "Total clocks done: " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Size() << "\n\n";
}

inline uint64_t std_benchmark_independ(DVector &vec1, DVector &vec2) {
    if (vec1.Size() == 0) return 0;
    register int raw_v1 asm("%ah") = 0;
    register int raw_v2 asm("%al") = 0;
    register double res_1 asm("%dx") = 0;
    register double res_2 asm("%dl") = 0;
    auto result = 0;
    for (std::size_t i = 0; i < vec1.Size(); ++i) {
        raw_v1 = vec1.GetRawBlock(i);
        raw_v2 = vec2.GetRawBlock(i);
        auto r1 = rdtsc();
        res_1 = static_cast<double>(raw_v1) + static_cast<double>(raw_v2);
        res_2 = static_cast<double>(raw_v1) + static_cast<double>(raw_v2);
        auto r2 = rdtsc();
        result += approx_rdtsc(r2 - r1);
    }
    return result;
}

inline void default_benchmark(DVector &vec1, DVector &vec2) {
    std::chrono::nanoseconds timer{0};
    register int raw_v1 asm("%ah") = 0;
    register int raw_v2 asm("%al") = 0;
    register double res_1 asm("%dx") = 0;
    register double res_2 asm("%dl") = 0;

    for (std::size_t i = 0; i < vec1.Size(); ++i) {
        raw_v1 = vec1[i];
        raw_v2 = vec2[i];
        auto start = std::chrono::high_resolution_clock::now();
        res_1 = static_cast<double>(raw_v1) + static_cast<double>(raw_v2);
        res_2 = static_cast<double>(raw_v1) + static_cast<double>(raw_v2);
        auto end = std::chrono::high_resolution_clock::now();

        timer += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    auto clocks_total = std_benchmark_independ(vec1, vec2);
    auto total_stdnow_clocks = vec1.Size() * AVG_CLOCS_PER_STDNOW;

    std::cout << "Default cast+add time " << timer.count() << " ns\n";
    std::cout << "Clocks per second: "
              << (clocks_total + static_cast<double>(total_stdnow_clocks)) / (timer.count() / 1000000000.0) << "\n";
    std::cout << "Total clocks done (without chrono::now): " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Size() << "\n";
}