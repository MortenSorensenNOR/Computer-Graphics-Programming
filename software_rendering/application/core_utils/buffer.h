#pragma once

#include <cstddef>

template <typename T>
struct Buffer {
    std::size_t size;
    T* data;
};

template <typename T>
Buffer<T> buffer_allocate(std::size_t size);

template <typename T>
int buffer_copy(Buffer<T>& from, Buffer<T>& to);

template <typename T>
int buffer_free(Buffer<T>& buffer);
