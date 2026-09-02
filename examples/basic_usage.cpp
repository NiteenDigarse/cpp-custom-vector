#include <iostream>
#include "custom_vector.hpp"

int main()
{
    CustomVector<int> numbers;

    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    std::cout << "Size: " << numbers.size() << '\n';
    std::cout << "Capacity: " << numbers.capacity() << '\n';

    std::cout << "Elements: ";

    for (int value : numbers)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';

    return 0;
}