CustomVector --- Memory & Object Lifetime

Project

Project: CustomVector
Tier: Tier 1 --- Foundation
Project Number: 01

1. Core Data Members

CustomVector<T> manages three important pieces of state:

T* data_;
std::size_t size_;
std::size_t capacity_;

data_

Points to the beginning of the allocated storage.

size_

Number of T objects that currently exist and are alive.

capacity_

Number of T objects that can fit in the currently allocated raw
storage.

Example:

capacity = 4
size     = 2

┌─────┬─────┬─────┬─────┐
│ 10  │ 20  │ RAW │ RAW │
└─────┴─────┴─────┴─────┘
  ↑     ↑
 live  live

Only the first two positions contain live objects.

2. Initial State

A newly created vector starts as:

data_     = nullptr;
size_     = 0;
capacity_ = 0;

This means:

No storage has been allocated.

No T objects exist.

The vector is empty.

No unnecessary initial heap allocation is performed.

3. Storage and Object Are Different

One of the most important concepts in this project is:

Allocated memory is not automatically a constructed object.

These are separate operations.

Memory lifetime
      ↓
Allocate raw storage
      ↓
Object lifetime
      ↓
Construct T object
      ↓
Use object
      ↓
Destroy T object
      ↓
Memory lifetime
      ↓
Release raw storage

Conceptually:

allocate
   ↓
RAW STORAGE
   ↓
construct_at
   ↓
LIVE OBJECT
   ↓
destroy_at
   ↓
RAW STORAGE
   ↓
operator delete
   ↓
MEMORY RELEASED

4. Raw Storage Allocation

The project uses:

::operator new(sizeof(T) * new_capacity);

This obtains raw storage.

It does not construct new_capacity objects of type T.

The helper is:

T* allocate(std::size_t new_capacity)
{
    return static_cast<T*>(
        ::operator new(sizeof(T) * new_capacity)
    );
}

Responsibility of allocate()

allocate() only:

Calculates required bytes.

Allocates raw storage.

Returns a T*.

It does not:

modify data_

modify size_

modify capacity_

construct any T object.

This keeps the helper focused on one responsibility.

5. Constructing Objects

Once raw storage exists, an actual T object can be created using:

std::construct_at(pointer, value);

Example:

std::construct_at(data_ + size_, value);

If:

size_ = 2

then:

data_ + size_

points to index 2.

That location is raw storage, so construct_at() creates the new object
there.

After successful construction:

++size_;

is performed because one additional object is now alive.

Important order

construct object
      ↓
construction succeeds
      ↓
size_++

Do not increase size_ before successful construction.

6. Destroying Objects

A live object is destroyed using:

std::destroy_at(data_ + i);

Destroying an object does not release the entire allocated storage.

After destruction, the location becomes raw storage again.

LIVE OBJECT
    ↓
destroy_at()
    ↓
RAW STORAGE

Only after all live objects have been destroyed should the raw storage
be released.

7. operator new and operator delete

This project deliberately uses matching low-level operations:

::operator new(...)

paired with:

::operator delete(...)

The conceptual lifetime is:

::operator new
      ↓
raw storage
      ↓
construct_at
      ↓
T object
      ↓
destroy_at
      ↓
raw storage
      ↓
::operator delete

destroy_at() and operator delete() have different responsibilities.

destroy_at() ends the lifetime of an object.

operator delete() releases the raw storage.

8. Reallocation

When the vector becomes full:

size_ == capacity_

new storage is required.

The current growth strategy is:

0 → 1 → 2 → 4 → 8 → 16 → 32 → ...

The reallocation flow is:

Old storage
    ↓
Allocate new raw storage
    ↓
Move existing objects
    ↓
Destroy old objects
    ↓
Release old raw storage
    ↓
data_ points to new storage
    ↓
capacity_ updated

Example:

Before:

data_
 ↓
┌────┬────┐
│ 10 │ 20 │
└────┴────┘
size = 2
capacity = 2

Reallocate to capacity 4:

new_data
   ↓
┌────┬────┬────┬────┐
│ 10 │ 20 │ RAW │ RAW│
└────┴────┴────┴────┘

Then the old objects are destroyed and old storage is released.

Finally:

data_ → new_data
capacity_ = 4

9. Why Reallocation Uses Move

During reallocation, existing objects must be transferred to the new
storage.

The implementation uses:

std::construct_at(
    new_data + i,
    std::move(data_[i])
);

std::move() itself does not move memory.

It allows the expression to participate in move construction when the
type supports it.

The actual move behavior is defined by T.

For a resource-owning type, moving can transfer ownership instead of
duplicating the resource.

10. Copy vs Move

Copy

Copying generally creates an independent object/resource state.

a → [10][20]

b → [10][20]

The storage is separate.

Move

Moving can transfer ownership of resources:

a → [resource]
        ↓
        transfer
        ↓
b → [resource]

a → valid moved-from state

The moved-from object remains alive and must remain valid for
destruction and other permitted operations.

11. Rule of Five

Because CustomVector manages a resource manually, ownership semantics
are important.

The class provides:

CustomVector(const CustomVector& other);            // Copy constructor

CustomVector& operator=(const CustomVector& other); // Copy assignment

CustomVector(CustomVector&& other) noexcept;        // Move constructor

CustomVector& operator=(CustomVector&& other) noexcept; // Move assignment

~CustomVector();                                    // Destructor

These five operations form the Rule of Five set.

The default constructor is separate from the Rule of Five.

12. Copy Constructor

The copy constructor creates new storage and copy-constructs the
elements.

Conceptually:

a → [10][20][30]

        COPY

b → [10][20][30]

The storage must be independent.

Therefore:

a.data_ != b.data_

while the element values are equivalent.

13. Copy Assignment

For:

b = a;

b already exists.

A safe design prepares the new copy first:

Prepare new storage
      ↓
Copy elements
      ↓
If successful:
destroy old b
      ↓
release old b storage
      ↓
install new storage

Self-assignment is handled:

if (this == &other)
{
    return *this;
}

so:

a = a;

does not destroy its own data before copying it.

14. Move Constructor

For:

CustomVector<int> b = std::move(a);

the vector can transfer ownership of its existing storage.

Conceptually:

Before:

a → [10][20]
      storage A


After:

b → [10][20]
      storage A

a → empty state

The move constructor therefore transfers:

data_
size_
capacity_

and resets the source:

other.data_ = nullptr;
other.size_ = 0;
other.capacity_ = 0;

15. Move Assignment

For:

b = std::move(a);

b may already own storage.

Therefore:

Destroy b's current objects
      ↓
Release b's current storage
      ↓
Take a's storage
      ↓
Reset a

Self-move is also checked:

if (this == &other)
{
    return *this;
}

16. Exception Safety During Reallocation

Reallocation can involve constructors or move constructors that may
throw.

The dangerous situation is partial construction:

new_data

┌────┬────┬────┬────┬────┐
│obj │obj │obj │ ?  │ ?  │
└────┴────┴────┴────┴────┘
                ↑
             exception

To handle this, the implementation tracks:

std::size_t constructed = 0;

After every successful construction:

++constructed;

If construction throws, only successfully constructed objects are
destroyed:

for (std::size_t i = 0; i < constructed; ++i)
{
    std::destroy_at(new_data + i);
}

Then the new raw storage is released:

::operator delete(new_data);

Finally:

throw;

re-throws the original exception.

Key distinction

For cleanup of new partially constructed storage:

i < constructed

For cleanup of all live old objects:

i < size_

These counts represent different things.

17. Destructor

The destructor follows the reverse lifetime order:

Live objects
    ↓
destroy_at()
    ↓
Raw storage
    ↓
operator delete()

Implementation:

~CustomVector()
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        std::destroy_at(data_ + i);
    }

    ::operator delete(data_);
}

Only size_ objects are destroyed.

capacity_ is not used for object destruction because unused capacity
contains raw storage, not live T objects.

18. push_back() and Object Lifetime

For:

v.push_back(value);

the basic flow is:

Check capacity
      ↓
Full?
  /       \
NO         YES
 |          |
 |       reallocate()
 |          |
 └────┬─────┘
      ↓
construct_at(data_ + size_, value)
      ↓
++size_

For the rvalue overload:

std::construct_at(
    data_ + size_,
    std::move(value)
);

This enables move construction where appropriate.

19. pop_back()

pop_back() removes the last live object.

std::destroy_at(data_ + (size_ - 1));
--size_;

It does not reduce capacity.

Example:

Before:

[10][20][30][RAW]
size = 3
capacity = 4


pop_back()


After:

[10][20][RAW][RAW]
size = 2
capacity = 4

20. clear()

clear() destroys all live objects:

for (std::size_t i = 0; i < size_; ++i)
{
    std::destroy_at(data_ + i);
}

size_ = 0;

It does not release the allocated storage.

Therefore:

After clear:

size     = 0
capacity = unchanged

This allows future insertions to reuse existing storage.

21. Access and Iteration

operator[]

T& operator[](std::size_t index)
{
    return data_[index];
}

It provides direct indexed access without bounds checking.

at()

at() checks:

if (index >= size_)
{
    throw std::out_of_range(...);
}

It protects against invalid element access.

Iterators

T* begin() noexcept
{
    return data_;
}

T* end() noexcept
{
    return data_ + size_;
}

end() points one position past the last live element.

22. Complexity

Operation                           Complexity

push_back()                       Amortized O(1)
push_back() during reallocation   O(n)
pop_back()                        O(1)
operator[]                        O(1)
at()                              O(1)
size()                            O(1)
capacity()                        O(1)
clear()                           O(n)
Copy construction                   O(n)
Copy assignment                     O(n)
Move construction                   O(1)
Move assignment                     O(1)

23. Engineering Lessons

This project demonstrates several important C++ engineering concepts:

Memory allocation and object construction are different operations.

Object lifetime must be managed explicitly when using raw storage.

Ownership must have a clear owner.

size_ represents live objects, not allocated bytes.

capacity_ represents available storage for objects.

Reallocation requires transferring existing objects before releasing
old storage.

Copy semantics create independent state.

Move semantics can transfer ownership efficiently.

Exception paths need their own cleanup logic.

RAII is essential for reliable resource management.

Performance should be measured rather than assumed.

24. Current Implementation Philosophy

The project intentionally builds the container progressively.

The goal is not to reproduce every detail of std::vector immediately.

The goal is to understand the engineering foundations first:

Memory
  ↓
Storage
  ↓
Object Lifetime
  ↓
Ownership
  ↓
Copy / Move
  ↓
Exception Safety
  ↓
API
  ↓
Testing
  ↓
Benchmarking

Future improvements can be added after the core implementation is
stable.

25. Final Mental Model

The most important mental model for CustomVector is:

                CustomVector
                     │
          ┌──────────┴──────────┐
          ↓                     ↓
      Raw Storage          Live Objects
          │                     │
       data_              size_ objects
          │
      capacity_
          │
          ↓
     ┌───────────────┐
     │ allocate      │
     │ construct     │
     │ use           │
     │ destroy       │
     │ deallocate    │
     └───────────────┘

The central rule is:

Storage lifetime and object lifetime are separate.

Understanding this distinction is the foundation for implementing
containers, allocators, memory pools, object pools, and many other
low-level C++ systems.