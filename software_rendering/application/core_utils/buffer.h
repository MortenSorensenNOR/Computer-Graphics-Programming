#pragma once

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
    if (buffer.data) {
        buffer.size = size;
    }
    return buffer;
}

template <typename T>
int buffer_copy(Buffer<T>& from, Buffer<T>& to) {
    if (from.size == 0 || to.size == 0){
        return 1;
    }

    if (from.size != to.size) {
        return 2;
    }

    if (!to.data || !from.data) {
        return 3;
    }

    memcpy(to.data, from.data, to.size);

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
