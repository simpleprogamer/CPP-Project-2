#pragma once
#include <algorithm>
#include <chrono>
#include <vector>

inline volatile bool resultSink = false;


template <class Action>
double measureMedianMs(Action&& action, int reps) {
    using Clock = std::chrono::steady_clock;
    action();
    std::vector<double> samples;
    samples.reserve(static_cast<std::size_t>(reps));
    for (int i = 0; i < reps; ++i) {
        const auto start = Clock::now();
        action();
        const auto stop = Clock::now();
        samples.push_back(std::chrono::duration<double, std::milli>(stop - start).count());
    }
    auto middle = samples.begin() + static_cast<std::ptrdiff_t>(samples.size() / 2);
    std::nth_element(samples.begin(), middle, samples.end());
    return *middle;
}
