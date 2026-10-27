#pragma once
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

using Sequence = std::vector<int>;

inline void writeSequence(const std::string& path, const Sequence& data) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("cannot open " + path);
    out << data.size() << '\n';
    for (std::size_t i = 0; i < data.size(); ++i)
        out << data[i] << (i + 1 < data.size() ? ' ' : '\n');
}

inline Sequence readSequence(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open " + path);
    std::size_t n = 0;
    in >> n;
    Sequence data(n);
    for (int& x : data) in >> x;
    return data;
}
