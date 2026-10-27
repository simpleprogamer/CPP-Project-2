#pragma once
#include <cmath>


struct IsPositive {
    static constexpr const char* kName = "cheap";
    static constexpr const char* kDescription = "x > 0";
    bool operator()(int x) const { return x > 0; }
};


struct IsPositiveExpensive {
    static constexpr const char* kName = "expensive";
    static constexpr const char* kDescription = "exp(log1p(x)) > 0.5";
    bool operator()(int x) const {
        return std::exp(std::log1p(static_cast<double>(x))) > 0.5;
    }
};
