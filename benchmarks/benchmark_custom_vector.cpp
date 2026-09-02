#include <iostream>
#include <chrono>
// #include "../include/custom_vector.hpp"
#include "custom_vector.hpp"

int main()
{
    constexpr std::size_t N = 1'000'000;

    CustomVector<int> v;

    auto start = std::chrono::high_resolution_clock::now();

    for (std::size_t i = 0; i < N; ++i)
    {
        v.push_back(static_cast<int>(i));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        );

    std::cout << "Elements: " << v.size() << '\n';
    std::cout << "Capacity: " << v.capacity() << '\n';
    std::cout << "Time: " << duration.count() << " ms\n";

    return 0;
}