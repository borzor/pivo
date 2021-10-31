#include <iostream>
#include <chrono>
#include "DVector.hpp"
#include "Constants.hpp"

inline uint64_t sse2_benchmark_independ(DVector &vec1, DVector &vec2) {
    auto result = 0;
    for (std::size_t i = 0, j = 0; i < vec1.Sse2Blocks(); ++i, j += vec1.Sse2BlockSize) {
        const DVector::SSE2_REG block1 = _mm_loadu_si128(&vec1.GetRawSse2Block(i));
        const DVector::SSE2_REG block2 = _mm_loadu_si128(&vec2.GetRawSse2Block(i));
        __m128d res,res1;
        auto r1 = rdtsc();
        res = _mm_add_pd(_mm_cvtepi32_pd(block1), _mm_cvtepi32_pd(block2));
        res1 = _mm_add_pd(_mm_cvtepi32_pd(block1), _mm_cvtepi32_pd(block2));
        auto r2 = rdtsc();
        result += approx_rdtsc(r2 - r1);
    }
    return result;
}

inline void sse2_benchmark_depend(DVector &vec1, DVector &vec2, DVector &vec3) {
    auto clocks_total = 0;
    for (std::size_t i = 0, j = 0; i < vec1.Sse2Blocks(); ++i, j += vec1.Sse2BlockSize) {
        const DVector::SSE2_REG block1 = _mm_loadu_si128(&vec1.GetRawSse2Block(i));
        const DVector::SSE2_REG block2 = _mm_loadu_si128(&vec2.GetRawSse2Block(i));
        __m128d res,res1;
        auto r1 = rdtsc();
        res = _mm_add_pd(_mm_cvtepi32_pd(block1), _mm_cvtepi32_pd(block2));
        res1 = _mm_add_pd(res, _mm_cvtepi32_pd(block2));
        auto r2 = rdtsc();
        clocks_total += approx_rdtsc(r2 - r1);
    }
    std::cout << "DEPENDENCE OPERATIONS:\n";
    std::cout << "Total clocks done: " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Sse2Blocks() << "\n\n";
}

inline void sse2_benchmark(DVector &vec1, DVector &vec2) {
    std::chrono::nanoseconds timer{0};
    for (std::size_t i = 0, j = 0; i < vec1.Sse2Blocks(); ++i, j += vec1.Sse2BlockSize) {
        const DVector::SSE2_REG block1 = _mm_loadu_si128(&vec1.GetRawSse2Block(i));
        const DVector::SSE2_REG block2 = _mm_loadu_si128(&vec2.GetRawSse2Block(i));
        auto start = std::chrono::high_resolution_clock::now();
        auto res = _mm_add_pd(_mm_cvtepi32_pd(block1), _mm_cvtepi32_pd(block2));
        auto res1 = _mm_add_pd(_mm_cvtepi32_pd(block1), _mm_cvtepi32_pd(block2));
        auto end = std::chrono::high_resolution_clock::now();
        timer += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }
    auto clocks_total = sse2_benchmark_independ(vec1, vec2);
    auto total_stdnow_clocks = vec1.Sse2Blocks() * AVG_CLOCS_PER_STDNOW;
    std::cout << "SSE2 cast+add time " << timer.count() << " ns\n";
    std::cout << "Clocks per second: "
              << (clocks_total + static_cast<double>(total_stdnow_clocks)) / (timer.count() / 1000000000.0) << "\n";
    std::cout << "Total clocks done (without chrono::now): " << clocks_total << "\n";
    std::cout << "Average clocks per operation " << clocks_total / vec1.Sse2Blocks() << "\n";
}