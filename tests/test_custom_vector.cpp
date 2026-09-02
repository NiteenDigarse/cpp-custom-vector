#include<iostream>
// #include "../include/custom_vector.hpp"
#include "custom_vector.hpp"

int main()
{
    CustomVector<int> v;

    // 1. Empty vector
    std::cout << "Initial size: "
              << v.size() << '\n';

    std::cout << "Initial capacity: "
              << v.capacity() << '\n';

    // 2. push_back()
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    std::cout << "After push_back:\n";

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        std::cout << *it << ' ';
    }

    std::cout << "\nSize: "
              << v.size() << '\n';

    std::cout << "Capacity: "
              << v.capacity() << '\n';

    // 3. operator[]
    std::cout << "v[1]: "
              << v[1] << '\n';

    // 4. at()
    std::cout << "v.at(2): "
              << v.at(2) << '\n';

    // 5. pop_back()
    v.pop_back();

    std::cout << "After pop_back:\n";

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        std::cout << *it << ' ';
    }

    std::cout << "\nSize: "
              << v.size() << '\n';

    // 6. clear()
    v.clear();

    std::cout << "After clear:\n";
    std::cout << "Size: "
              << v.size() << '\n';

    std::cout << "Capacity: "
              << v.capacity() << '\n';


// -----------------------------
// Copy Constructor Test


CustomVector<int> a;

a.push_back(10);
a.push_back(20);

CustomVector<int> b = a;

std::cout << "\nCopy Constructor Test:\n";

std::cout << "a: ";
for (auto it = a.begin(); it != a.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\nb: ";
for (auto it = b.begin(); it != b.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << '\n';



a[0] = 100;

std::cout << "\nAfter modifying a:\n";

std::cout << "a[0]: " << a[0] << '\n';
std::cout << "b[0]: " << b[0] << '\n';


// Testing the move constructor
CustomVector<int> c = std::move(a);

std::cout << "\nMove Constructor Test:\n";

std::cout << "c: ";
for (auto it = c.begin(); it != c.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\na size: " << a.size() << '\n';
std::cout << "c size: " << c.size() << '\n';


// -----------------------------
// Copy Assignment Test
// -----------------------------

CustomVector<int> x;
x.push_back(100);

CustomVector<int> y;
y.push_back(500);

y = x;

std::cout << "\nCopy Assignment Test:\n";

std::cout << "x: ";
for (auto it = x.begin(); it != x.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\ny: ";
for (auto it = y.begin(); it != y.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << '\n';

// Check independent storage
x[0] = 200;

std::cout << "After modifying x:\n";
std::cout << "x[0]: " << x[0] << '\n';
std::cout << "y[0]: " << y[0] << '\n';


// -----------------------------
// Move Assignment Test
// -----------------------------

CustomVector<int> p;

p.push_back(10);
p.push_back(20);

CustomVector<int> q;
q.push_back(100);

q = std::move(p);

std::cout << "\nMove Assignment Test:\n";

std::cout << "q: ";
for (auto it = q.begin(); it != q.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\np size: " << p.size() << '\n';
std::cout << "q size: " << q.size() << '\n';


// -----------------------------
// at() Out-of-Range Test
// -----------------------------

CustomVector<int> test;

test.push_back(10);
test.push_back(20);

try
{
    std::cout << "\ntest.at(5): "
              << test.at(5) << '\n';
}
catch (const std::out_of_range& e)
{
    std::cout << "\nException caught: "
              << e.what() << '\n';
}


// -----------------------------
// Self Assignment Test
// -----------------------------

CustomVector<int> self;

self.push_back(10);
self.push_back(20);

self = self;

std::cout << "\nSelf Assignment Test:\n";

for (auto it = self.begin(); it != self.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\nSize: "
          << self.size() << '\n';

  // self move cheking 

    self = std::move(self);

std::cout << "\nSelf Move Test:\n";

for (auto it = self.begin(); it != self.end(); ++it)
{
    std::cout << *it << ' ';
}

std::cout << "\nSize: "
          << self.size() << '\n';


          // -----------------------------
// Object Lifetime Test
// -----------------------------

class Tracker
{
public:
    int value;

    Tracker(int v)
        : value(v)
    {
        std::cout << "Construct: " << value << '\n';
    }

    Tracker(const Tracker& other)
        : value(other.value)
    {
        std::cout << "Copy: " << value << '\n';
    }

    Tracker(Tracker&& other) noexcept
        : value(other.value)
    {
        std::cout << "Move: " << value << '\n';
    }

    ~Tracker()
    {
        std::cout << "Destroy: " << value << '\n';
    }
};

std::cout << "\nObject Lifetime Test:\n";

{
    CustomVector<Tracker> objects;

    objects.push_back(Tracker(10));
    objects.push_back(Tracker(20));
    objects.push_back(Tracker(30));

    std::cout << "Leaving scope...\n";
}

std::cout << "Vector destroyed successfully.\n";
    return 0;
}