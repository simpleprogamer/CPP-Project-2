#pragma once
#include <cstddef>
#include <vector>

struct LinearFit {
    double intercept;  
    double slope;      
    double r2;         
};


inline LinearFit fitLinear(const std::vector<double>& x, const std::vector<double>& y) {
    const double n = static_cast<double>(x.size());
    double sx = 0, sy = 0, sxx = 0, sxy = 0;
    for (std::size_t i = 0; i < x.size(); ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        sxy += x[i] * y[i];
    }
    const double slope = (n * sxy - sx * sy) / (n * sxx - sx * sx);
    const double intercept = (sy - slope * sx) / n;
    double ssRes = 0, ssTot = 0;
    const double mean = sy / n;
    for (std::size_t i = 0; i < x.size(); ++i) {
        const double fit = intercept + slope * x[i];
        ssRes += (y[i] - fit) * (y[i] - fit);
        ssTot += (y[i] - mean) * (y[i] - mean);
    }
    return {intercept, slope, ssTot > 0 ? 1.0 - ssRes / ssTot : 1.0};
}
