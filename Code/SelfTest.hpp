#pragma once
#include <algorithm>
#include <iostream>
#include <random>

#include "ChunkedAllOf.hpp"
#include "Predicates.hpp"
#include "SequenceFile.hpp"

inline bool runSelfTest() {
    std::mt19937 engine(12345);
    std::uniform_int_distribution<int> lengthDist(0, 300), kDist(1, 40), valueDist(1, 1000);
    std::bernoulli_distribution injectViolation(0.5);
    const IsPositive pred;
    for (int trial = 0; trial < 2000; ++trial) {
        Sequence data(static_cast<std::size_t>(lengthDist(engine)));
        for (int& x : data) x = valueDist(engine);
        if (!data.empty() && injectViolation(engine)) {
            std::uniform_int_distribution<std::size_t> pos(0, data.size() - 1);
            data[pos(engine)] = -valueDist(engine);
        }
        const auto k = static_cast<std::size_t>(kDist(engine));
        if (chunkedAllOf(data.begin(), data.end(), pred, k) !=
            std::all_of(data.begin(), data.end(), pred)) {
            std::cout << "Self-test FAILED (trial " << trial << ")\n";
            return false;
        }
    }
    std::cout << "Self-test passed: own all_of == std::all_of on 2000 random cases\n\n";
    return true;
}
