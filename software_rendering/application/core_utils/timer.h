#pragma once

#include <chrono>

enum class TimerType {
    SECOND,
    MILLISECOND,
    MICROSECOND,
    NANOSECOND,
};


class Timer {
private:
    TimerType type;

    std::chrono::time_point<std::chrono::high_resolution_clock> timer_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> timer_end;

public:
    Timer() : timer_start(), timer_end() {} 

    void start() {
        timer_start = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        timer_end = std::chrono::high_resolution_clock::now();
    }

    float get() {
        const auto time_nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(timer_end - timer_start);
        return time_nanosec.count() / 1000.0f;
    }
};
