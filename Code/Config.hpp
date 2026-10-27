#pragma once

#include <array>
#include <cstddef>
#include <string>

namespace config {


inline constexpr std::array<std::size_t, 3> kSequenceSizes{100'000, 1'000'000, 10'000'000};

inline constexpr unsigned kSeed = 20260101;  
inline constexpr int kMinValue = 1;         
inline constexpr int kMaxValue = 1'000'000;  

inline std::string sequenceFileName(std::size_t n) {
    return "seq_" + std::to_string(n) + ".txt";
}


inline int repetitionsFor(std::size_t n) {
    if (n <= 100'000) return 31;
    if (n <= 1'000'000) return 15;
    return 7;
}

}

