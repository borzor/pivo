#include "raw.cpp"
#include "SSE.cpp"
#include "avx.cpp"
std::size_t digits_count = 0;


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Digits count expected\n";
        return -1;
    }
    digits_count = std::stoi(argv[1]);

    DVector vec1(digits_count);
    DVector vec2(digits_count);
    DVector vec3(digits_count);

    default_benchmark(vec1, vec2);
    std_benchmark_depend(vec1, vec2, vec3);

    sse2_benchmark(vec1, vec2);
    sse2_benchmark_depend(vec1, vec2, vec3);

    avx2_benchmark(vec1, vec2);
    avx2_benchmark_depend(vec1, vec2, vec3);

    return 0;
}
