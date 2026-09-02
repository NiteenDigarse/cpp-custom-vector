# CustomVector

A custom implementation of a dynamic array in modern C++.

The goal of this project is to understand how a vector-like container manages
dynamic memory, object lifetime, copy/move semantics, reallocation, and
exception safety internally.

---

## Features

- Generic `CustomVector<T>`
- Dynamic storage allocation
- Automatic capacity growth
- `size()` and `capacity()`
- `push_back()` for lvalues and rvalues
- `pop_back()`
- `clear()`
- `operator[]`
- Bounds-checked `at()`
- `begin()` / `end()`
- Copy constructor
- Copy assignment
- Move constructor
- Move assignment
- Manual object lifetime management
- Exception-safe reallocation
- RAII-based resource management

---

## Memory Model

`CustomVector` maintains three important members:

```cpp
T* data_;
std::size_t size_;
std::size_t capacity_;



size_

Number of objects that are currently alive.

capacity_

Number of T objects that can fit in the currently allocated storage.

Example:

capacity = 4
size     = 2

┌─────┬─────┬─────┬─────┐
│ 10  │ 20  │ RAW │ RAW │
└─────┴─────┴─────┴─────┘

Only two T objects currently exist.

Storage vs Object Lifetime

The project deliberately separates raw storage from object lifetime.

Allocate raw storage
        ↓
Construct objects
        ↓
Use objects
        ↓
Destroy objects
        ↓
Release raw storage

Raw memory is obtained using:

::operator new(...)

Objects are constructed using:

std::construct_at(...)

Objects are destroyed using:

std::destroy_at(...)

Raw storage is released using:

::operator delete(...)
Growth Strategy

When the vector becomes full, capacity grows approximately by 2x.

0 → 1 → 2 → 4 → 8 → 16 → 32 → ...

This reduces the number of allocations required during repeated
push_back() operations.

Reallocation

When the current storage is full:

Old storage
    ↓
Allocate new storage
    ↓
Move existing objects
    ↓
Destroy old objects
    ↓
Release old storage
    ↓
Update data_ and capacity_

During reallocation, successfully constructed objects are tracked so that
partially completed construction can be cleaned up if an exception occurs.

Copy Semantics

Copying creates independent storage.

CustomVector<int> a;
CustomVector<int> b = a;

Conceptually:

a → [10][20]

b → [10][20]

The objects contain the same values, but the underlying storage is separate.

Move Semantics

Moving transfers ownership of the existing storage instead of copying every
element.

CustomVector<int> b = std::move(a);

Conceptually:

Before:

a → [10][20]


After:

b → [10][20]

a → empty state

The moved-from object remains valid and is reset to an empty state.

Exception Safety

Reallocation tracks how many objects have been successfully constructed in
the new storage.

If construction fails:

Destroy successfully constructed objects
            ↓
Release new storage
            ↓
Re-throw exception

This prevents the newly allocated storage from leaking.

Complexity
Operation	Complexity
push_back()	Amortized O(1)
push_back() during reallocation	O(n)
pop_back()	O(1)
operator[]	O(1)
at()	O(1)
size()	O(1)
capacity()	O(1)
clear()	O(n)
Benchmark

A basic benchmark was performed using:

1,000,000 push_back() operations

Observed runtime on the development machine:

~18 ms

The exact result depends on hardware, compiler, optimization level, and
system load.

Project Structure
01-custom-vector/
├── include/
│   └── custom_vector.hpp
│
├── src/
│
├── tests/
│   └── test_custom_vector.cpp
│
├── examples/
│
├── benchmarks/
│   └── benchmark_custom_vector.cpp
│
├── notes/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
Build

Example using g++:

g++ -std=c++20 tests/test_custom_vector.cpp -o test_vector

Run:

./test_vector

Benchmark:

g++ -std=c++20 -O2 benchmarks/benchmark_custom_vector.cpp -o benchmark

Run:

./benchmark
Learning Goals

This project focuses on understanding:

Dynamic memory management
Raw storage
Object lifetime
Placement construction
Destruction
Ownership
RAII
Copy semantics
Move semantics
Exception safety
Capacity growth
Amortized complexity
Generic programming with templates
Future Improvements

Potential future improvements include:

emplace_back()
More complete iterator support
Allocator support
Better growth-policy benchmarking
Additional exception-safety guarantees
More comprehensive automated tests
Performance comparison with std::vector