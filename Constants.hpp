#pragma once
#include <cstdlib>

//Среднее число тактов на функцию подсчета времени
#define AVG_CLOCS_PER_STDNOW 65


//Функция подсчета процессорных тактов
static inline uint64_t rdtsc()
{
    unsigned int lo, hi;
    asm volatile ( "rdtsc\n" : "=a" (lo), "=d" (hi) );
    return ((uint64_t)hi << 32) | lo;
}

//Функция аппроксимации подсчета тактов (вычет тактов на вызов функции)
static uint64_t approx_rdtsc(uint64_t clocks)
{
    auto call = rdtsc();
    auto delta = rdtsc() - call;

    if(clocks >= delta) return clocks - delta;
    else return clocks;
}