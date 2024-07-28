#pragma once

#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <string.h>

template <typename T>
struct Buffer {
    std::size_t size;
    T* data;
};

template <typename T>
Buffer<T> buffer_allocate(std::size_t size) {
    Buffer<T> buffer;
    buffer.data = (T*)malloc(size * sizeof(T));
    if (buffer.data != NULL) {
        buffer.size = size;
    } else {
        throw std::runtime_error("Could not allocate buffer of size " + std::to_string(size));
    }
    return buffer;
}

template <typename T>
int buffer_copy(Buffer<T>& from, Buffer<T>& to) {
    if (from.size == 0 || to.size == 0){
        throw std::runtime_error("buffer_copy: Buffer size equal to zero");
        return 1;
    }

    if (from.size != to.size) {
        throw std::runtime_error("buffer_copy: From size not equal to to size");
        return 2;
    }

    if (!to.data || !from.data) {
        throw std::runtime_error("buffer_copy: uninitialized buffers");
        return 3;
    }

    memcpy(to.data, from.data, to.size * sizeof(T));

    return 0;
}

template <typename T>
int buffer_fill(Buffer<T>& buffer, T value) {
    for (std::size_t i = 0; i < buffer.size; ++i) {
        buffer.data[i] = value;
    }
    return 0;
}

template <typename T>
int buffer_free(Buffer<T>& buffer) {
    if (buffer.size == 0) {
        return 1;
    }

    if (!buffer.data) {
        return 2;
    }

    free(buffer.data);
    return 0;
}
