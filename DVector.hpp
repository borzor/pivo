#pragma once
#include <vector>
#include <immintrin.h>
#include <emmintrin.h>
#include <random>

class DVector
{

public:

    using SSE2_REG = __m128i;
    using AVX2_REG = __m256i;

    explicit DVector(std::size_t volume)
    {
        STD.resize(volume);
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distrib(-2147483648,  2147483647);
        for (std::size_t i = 0; i < volume; ++i)
        {
            STD[i] = distrib(generator);
        }

        ConvertToSse2();
        ConvertToAvx2();
    }

    const int& operator[](std::size_t i) const
    {
        return STD[i];
    }

    [[nodiscard]] const std::vector<SSE2_REG>& AsSse2() const
    {
        return SSE2;
    }

    [[nodiscard]] const std::vector<AVX2_REG>& AsAvx2() const
    {
        return AVX2;
    }

    [[nodiscard]] const std::vector<int>& AsStd() const
    {
        return STD;
    }

    [[nodiscard]] std::size_t Sse2Blocks() const
    {
        return SSE2.size();
    }

    [[nodiscard]] std::size_t Avx2Blocks() const
    {
        return AVX2.size();
    }

    [[nodiscard]] std::size_t Size() const
    {
        return STD.size();
    }

    int* GetAvx2Block(std::size_t block_num)
    {
        return reinterpret_cast<int*>(&AVX2[block_num]);
    }

    int* GetSse2Block(std::size_t block_num)
    {
        return reinterpret_cast<int*>(&SSE2[block_num]);
    }

    [[nodiscard]] const AVX2_REG& GetRawAvx2Block(std::size_t block_num) const
    {
        return AVX2[block_num];
    }

    [[nodiscard]] const SSE2_REG& GetRawSse2Block(std::size_t block_num) const
    {
        return SSE2[block_num];
    }

    [[nodiscard]] const int& GetRawBlock(std::size_t block_num) const
    {
        return STD[block_num];
    }


    [[nodiscard]] const int* GetRawStdArr() const
    {
        return STD.data();
    }

    const std::size_t Sse2BlockSize = 4;
    const std::size_t Avx2BlockSize = 8;

private:

    void ConvertToSse2()
    {
        auto blocks = STD.size() - STD.size() % Sse2BlockSize;
        for (std::size_t i = 0; i < blocks; i += Sse2BlockSize)
        {
            SSE2.push_back(_mm_load_si128((__m128i*)&STD[i]));
        }

        for (auto last_byte = blocks; last_byte < STD.size(); last_byte += Sse2BlockSize)  SSE2.push_back(_mm_load_si128((__m128i*)(&STD[last_byte])));
    }


    void ConvertToAvx2()
    {

        auto blocks = STD.size() - STD.size() % Avx2BlockSize;
        for (std::size_t i = 0; i < blocks; i += Avx2BlockSize)
        {
            AVX2.push_back(_mm256_loadu_si256((__m256i*)&STD[i]));
        }

        for (auto last_byte = blocks; last_byte < STD.size(); last_byte += Avx2BlockSize)  AVX2.push_back(_mm256_loadu_si256((__m256i*)(&STD[last_byte])));
    }

    std::vector<int> STD;
    std::vector<SSE2_REG> SSE2;
    std::vector<AVX2_REG> AVX2;
};