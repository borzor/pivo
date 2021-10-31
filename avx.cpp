#include <iostream>
#include <chrono>
#include "DVector.hpp"
#include "Constants.hpp"

inline void avx2_benchmark_depend(DVector &vec1, DVector &vec2, DVector &vec3) {
    auto clocks_total = 0;
    for (std::size_t i = 0, j = 0; i < vec1.Avx2Blocks(); ++i, j += vec1.Avx2BlockSize) {
        const DVector::AVX2_REG block1 = _mm256_loadu_si256(&vec1.GetRawAvx2Block(i));
        const DVector::AVX2_REG block2 = _mm256_loadu_si256(&vec2.GetRawAvx2Block(i));
        const DVector::AVX2_REG block3 = _mm256_loadu_si256(&vec3.GetRawAvx2Block(i));
        auto r1 = rdtsc();
        auto res = _mm256_add_pd(_mm256_castsi256_pd(block1), _mm256_castsi256_pd(block2));
        auto res1 = _mm256_add_pd(res, _mm256_castsi256_pd(block2));
        auto r2 = rdtsc();
        clocks_total += approx_rdtsc(r2 - r1);
    }
    std::cout << "DEPENDENCE OPERATIONS:\n";
    std::cout << "Total clocks done: " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Avx2Blocks() << "\n\n";
}

inline uint64_t avx2_benchmark_independ(DVector &vec1, DVector &vec2) {
    auto result = 0;
    for (std::size_t i = 0, j = 0; i < vec1.Avx2Blocks(); ++i, j += vec1.Avx2BlockSize) {
        const DVector::AVX2_REG block1 = _mm256_loadu_si256(&vec1.GetRawAvx2Block(i));
        const DVector::AVX2_REG block2 = _mm256_loadu_si256(&vec2.GetRawAvx2Block(i));
        auto r1 = rdtsc();
        auto res = _mm256_add_pd(_mm256_castsi256_pd(block1), _mm256_castsi256_pd(block2));
        auto res1 = _mm256_add_pd(_mm256_castsi256_pd(block1), _mm256_castsi256_pd(block2));
        auto r2 = rdtsc();
        result += approx_rdtsc(r2 - r1);
    }
    return result;
}

inline void avx2_benchmark(DVector &vec1, DVector &vec2) {
    std::chrono::nanoseconds timer{0};
    for (std::size_t i = 0, j = 0; i < vec1.Avx2Blocks(); ++i, j += vec1.Avx2BlockSize) {
        const DVector::AVX2_REG block1 = _mm256_loadu_si256(&vec1.GetRawAvx2Block(i));
        const DVector::AVX2_REG block2 = _mm256_loadu_si256(&vec2.GetRawAvx2Block(i));
        auto start = std::chrono::high_resolution_clock::now();
        auto res = _mm256_add_pd(_mm256_castsi256_pd(block1), _mm256_castsi256_pd(block2));
        auto res1 = _mm256_add_pd(_mm256_castsi256_pd(block1), _mm256_castsi256_pd(block2));
        auto end = std::chrono::high_resolution_clock::now();
        timer += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }
    auto clocks_total = avx2_benchmark_independ(vec1, vec2);
    auto total_stdnow_clocks = vec1.Avx2Blocks() * AVG_CLOCS_PER_STDNOW;
    std::cout << "AVX2 cast+add time " << timer.count() << " ns\n";
    std::cout << "Clocks per second: "
              << (clocks_total + static_cast<double>(total_stdnow_clocks)) / (timer.count() / 1000000000.0) << "\n";
    std::cout << "Total clocks done (without chrono::now): " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Avx2Blocks() << "\n";

}


