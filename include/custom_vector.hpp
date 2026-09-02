#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <stdexcept>

template <typename T>
class CustomVector
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    // Allocate raw storage.
    // Does NOT construct any T objects.
    T* allocate(std::size_t new_capacity)
    {
        return static_cast<T*>(
            ::operator new(sizeof(T) * new_capacity)
        );
    }

    // Reallocate storage and move existing objects
    // into the new storage.
    void reallocate(std::size_t new_capacity)
    {
        // 1. Allocate new raw storage.
        T* new_data = allocate(new_capacity);

        // Number of objects successfully constructed
        // in the new storage.
        std::size_t constructed = 0;

        try
        {
            // 2. Move-construct existing objects
            //    into the new storage.
            for (std::size_t i = 0; i < size_; ++i)
            {
                std::construct_at(
                    new_data + i,
                    std::move(data_[i])
                );

                ++constructed;
            }
        }
        catch (...)
        {
            // 3. Destroy successfully constructed objects.
            for (std::size_t i = 0; i < constructed; ++i)
            {
                std::destroy_at(new_data + i);
            }

            // 4. Release new raw storage.
            ::operator delete(new_data);

            // 5. Re-throw the original exception.
            throw;
        }

        // 6. Destroy old objects.
        for (std::size_t i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }

        // 7. Release old raw storage.
        ::operator delete(data_);

        // 8. Point data_ to new storage.
        data_ = new_data;

        // 9. Update capacity.
        capacity_ = new_capacity;
    }

public:
    // Default constructor.
    CustomVector()
        : data_(nullptr),
          size_(0),
          capacity_(0)
    {
    }

    // Copy constructor.

    CustomVector(const CustomVector& other)
        : data_(nullptr),
          size_(0),
          capacity_(0)
    {
        if (other.capacity_ == 0)
        {
            return;
        }

        data_ = allocate(other.capacity_);

        try
        {
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                std::construct_at(
                    data_ + i,
                    other.data_[i]
                );

                ++size_;
            }

            capacity_ = other.capacity_;
        }
        catch (...)
        {
            for (std::size_t i = 0; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }

            ::operator delete(data_);

            throw;
        }
    }

    //Move constructor
    CustomVector(CustomVector&& other) noexcept
    : data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

//Copy Assignment 

CustomVector& operator=(const CustomVector& other)
{
    // Self-assignment check.
    if (this == &other)
    {
        return *this;
    }

    // Prepare new storage.
    T* new_data = nullptr;
    std::size_t new_size = 0;

    if (other.capacity_ != 0)
    {
        new_data = allocate(other.capacity_);

        try
        {
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                std::construct_at(
                    new_data + i,
                    other.data_[i]
                );

                ++new_size;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < new_size; ++i)
            {
                std::destroy_at(new_data + i);
            }

            ::operator delete(new_data);

            throw;
        }
    }

    // Destroy current objects.
    for (std::size_t i = 0; i < size_; ++i)
    {
        std::destroy_at(data_ + i);
    }

    // Release current storage.
    ::operator delete(data_);

    // Take ownership of new storage.
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;

    return *this;
}

//Move Assignment Operator

CustomVector& operator=(CustomVector&& other) noexcept
{
    // Self-move assignment check.
    if (this == &other)
    {
        return *this;
    }

    // Destroy current objects.
    for (std::size_t i = 0; i < size_; ++i)
    {
        std::destroy_at(data_ + i);
    }

    // Release current storage.
    ::operator delete(data_);

    // Transfer ownership.
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    // Leave other in a valid empty state.
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

//Basic operations 
std::size_t size() const noexcept
{
    return size_;
}

std::size_t capacity() const noexcept
{
    return capacity_;
}

bool empty() const noexcept
{
    return size_ == 0;
}

//Operator[] Accessing element of vector
T& operator[](std::size_t index)
{
    return data_[index];
}

const T& operator[](std::size_t index) const
{
    return data_[index];
}


// Writing the function of at() Also using in header #include <stdexcept> 
T& at(std::size_t index)
{
    if (index >= size_)
    {
        throw std::out_of_range(
            "CustomVector::at() index out of range"
        );
    }

    return data_[index];
}

const T& at(std::size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range(
            "CustomVector::at() index out of range"
        );
    }

    return data_[index];
}


// Pop back 
void pop_back()
{
    if (size_ == 0)
    {
        return;
    }

    std::destroy_at(data_ + (size_ - 1));

    --size_;
}

// Clear 
void clear()
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        std::destroy_at(data_ + i);
    }

    size_ = 0;
}

//begin() end()
T* begin() noexcept
{
    return data_;
}

const T* begin() const noexcept
{
    return data_;
}

T* end() noexcept
{
    return data_ + size_;
}

const T* end() const noexcept
{
    return data_ + size_;
}

    // Push an lvalue by copying.
    void push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            std::size_t new_capacity =
                (capacity_ == 0) ? 1 : capacity_ * 2;

            reallocate(new_capacity);
        }

        std::construct_at(
            data_ + size_,
            value
        );

        ++size_;
    }

    // Push an rvalue by moving.
    void push_back(T&& value)
    {
        if (size_ == capacity_)
        {
            std::size_t new_capacity =
                (capacity_ == 0) ? 1 : capacity_ * 2;

            reallocate(new_capacity);
        }

        std::construct_at(
            data_ + size_,
            std::move(value)
        );

        ++size_;
    }

    // Destructor.
    ~CustomVector()
    {
        // Destroy all live objects.
        for (std::size_t i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }

        // Release raw storage.
        ::operator delete(data_);
    }
};