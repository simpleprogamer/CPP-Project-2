#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <thread>
#include <vector>

template <class RandomIt, class Pred>
bool chunkedAllOf(RandomIt first, RandomIt last, Pred pred, std::size_t k) {
    const auto n = static_cast<std::size_t>(std::distance(first, last));
    k = std::clamp<std::size_t>(k, 1, std::max<std::size_t>(n, 1));

    std::vector<char> partial(k, 1);  
    std::vector<std::thread> workers;
    workers.reserve(k);

    const std::size_t base = n / k;  
    const std::size_t extra = n % k;  
    RandomIt begin = first;
    for (std::size_t i = 0; i < k; ++i) {
        RandomIt end = begin + static_cast<std::ptrdiff_t>(base + (i < extra ? 1 : 0));
        workers.emplace_back([begin, end, pred, i, &partial] {
            partial[i] = std::all_of(begin, end, pred);
        });
        begin = end;
    }
    for (std::thread& worker : workers) worker.join();

    return std::all_of(partial.begin(), partial.end(), [](char c) { return c != 0; });
}
