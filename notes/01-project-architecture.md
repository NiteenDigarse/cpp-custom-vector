Project Architecture --- CustomVector

Project

Project: CustomVectorTier: Tier 1 --- FoundationProject Number: 01

The goal of this project is to build a production-oriented, vector-likedynamic array in modern C++ and understand the engineering decisionsbehind a container similar in purpose to std::vector.

1. Project Structure

01-custom-vector/
│
├── include/
│   └── custom_vector.hpp
│
├── src/
│
├── tests/
│
├── examples/
│
├── benchmarks/
│
├── notes/
│   └── 01-project-architecture.md
│
├── CMakeLists.txt
├── README.md
└── .gitignore

2. Responsibility of Each Directory

include/

Contains the public interface of the library.

For this project:

include/
└── custom_vector.hpp

This is where users will access the CustomVector<T> API.

Think of it as the public face of the library.

src/

Contains implementation/source files when the project architecturerequires separate source translation units.

For a template class such as:

template <typename T>
class CustomVector

the implementation generally needs to remain visible to the compiler.Therefore, the initial template implementation will live in the header.

The src/ directory is still kept because larger projects may containnon-template implementation code later.

tests/

Contains correctness tests.

The purpose is to answer:

Does the implementation behave correctly?

Examples of future tests:

push_back()
pop_back()
size()
capacity()
reserve()
resize()
copy construction
copy assignment
move construction
move assignment
iterators
exception safety

Tests also help detect regressions when existing code is modified.

examples/

Contains small programs demonstrating how a developer uses the library.

Example:

CustomVector<int> numbers;

numbers.push_back(10);
numbers.push_back(20);

The purpose of examples is:

Show how to use the API.

This is different from tests.

tests/ → Is it correct?

examples/ → How do I use it?

benchmarks/

Contains performance measurement programs.

The purpose is to answer:

How fast is the implementation?

Eventually we can compare:

CustomVector
vs
std::vector

for operations such as:

push_back
iteration
random access
copy
move
reserve

Important distinction:

Tests      → correctness
Benchmarks → performance

3. CMakeLists.txt

CMakeLists.txt defines how the project is built.

It will eventually describe:

Project name

C++ standard

Library target

Test targets

Example targets

Benchmark targets

Build configuration

The project will use CMake rather than relying only on manual compilercommands.

Typical build flow:

cmake -S . -B build
cmake --build build

4. README.md

The README is the main documentation page of the repository.

It will eventually contain:

Project overview

Features

Design goals

Build instructions

Usage examples

Testing instructions

Benchmark results

Design decisions

Limitations

Future improvements

The README should allow another developer to understand and build theproject without reading the entire source code first.

5. .gitignore

.gitignore tells Git which generated or local files should not becommitted.

Typical examples:

build/
.vscode/
*.exe
*.obj
*.o

Source code, tests, notes and documentation should remain tracked.

6. Why This Structure?

A serious engineering project should separate differentresponsibilities.

include/
    Public API

src/
    Implementation

tests/
    Correctness

examples/
    Usage

benchmarks/
    Performance

notes/
    Engineering knowledge and design decisions

This separation makes a project easier to:

understand

test

maintain

debug

benchmark

extend

review

publish on GitHub

7. Engineering Principle

The project is not just about writing code.

For every major component we should understand:

WHY
 ↓
Architecture
 ↓
Implementation
 ↓
Memory
 ↓
Object lifetime
 ↓
Testing
 ↓
Performance
 ↓
Documentation

The notes/ directory exists so that the reasoning behind theimplementation is preserved.

8. Current Project Goal

The initial goal is to build a vector-like container with:

template <typename T>
class CustomVector;

The implementation will progressively cover:

Initial state

Dynamic storage

Size and capacity management

Element construction/destruction

push_back

pop_back

reserve

resize

Copy semantics

Move semantics

emplace_back

Iterators

Exception safety

Performance benchmarking

The project will become progressively more advanced rather thanimplementing everything at once.

9. Current Design Foundation

The initial class state is:

template <typename T>
class CustomVector
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
};

The intended meaning is:

data_
    → beginning of allocated storage

size_
    → number of currently existing/logical elements

capacity_
    → amount of storage available for elements

Initial empty state:

data_     = nullptr
size_     = 0
capacity_ = 0

This means an empty vector does not perform an unnecessary initial heapallocation.

10. Important Design Direction

This project will distinguish between:

Memory allocation
        and
Object construction

These are not the same operation.

The eventual design will conceptually follow:

Allocate raw storage
        ↓
Construct objects when needed
        ↓
Use objects
        ↓
Destroy objects
        ↓
Release raw storage

This distinction is one of the important engineering concepts behind aserious vector implementation.

Status

Completed in this note:

Project architecture

Directory responsibilities

Public API concept

Testing concept

Benchmarking concept

Documentation strategy

Initial CustomVector design

Next implementation topic:

Memory allocation and the allocate() helper.