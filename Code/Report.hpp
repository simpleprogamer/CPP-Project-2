#pragma once

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "Stats.hpp"

namespace report {

inline std::string compilerName() {
#if defined(_MSC_VER)
    return "MSVC " + std::to_string(_MSC_VER);
#elif defined(__clang__)
    return std::string("clang ") + __clang_version__;
#elif defined(__GNUC__)
    return std::string("GCC ") + __VERSION__;
#else
    return "unknown compiler";
#endif
}

inline void banner(const std::string& label, unsigned hardwareThreads) {
    std::cout << "Compiler: " << compilerName() << '\n'
              << "Build label: " << label << '\n'
              << "Hardware threads: " << hardwareThreads << "\n\n";
}

inline void sectionHeader(std::size_t n, const std::string& file, const char* name,
                          const char* description) {
    std::cout << "=== N = " << n << " | predicate: " << name << " (" << description
              << ") | file: " << file << " ===\n";
}

inline void libraryHeader(int reps) {
    std::cout << "-- Library std::all_of (median of " << reps << " runs)\n";
}

inline void libraryRow(const std::string& policy, double ms, bool result) {
    std::cout << "  " << std::left << std::setw(10) << policy << std::right << std::fixed
              << std::setprecision(4) << std::setw(12) << ms << " ms   result="
              << (result ? "true" : "false") << '\n';
}

inline void ownHeader(double sequentialMs, int reps) {
    std::cout << "-- Own chunked all_of, K threads (median of " << reps
              << " runs); sequential library baseline = " << std::fixed << std::setprecision(4)
              << sequentialMs << " ms\n";
}

inline void ownRow(std::size_t k, double ms, double speedup) {
    std::cout << "  K=" << std::setw(5) << k << std::setw(12) << std::fixed
              << std::setprecision(4) << ms << " ms   speedup " << std::setprecision(2)
              << speedup << "x\n";
}

inline void ownSummary(std::size_t bestK, double bestMs, unsigned hardwareThreads) {
    std::cout << "BEST K = " << bestK << " | time " << std::fixed << std::setprecision(4)
              << bestMs << " ms | hardware threads = " << hardwareThreads
              << " | K/threads = " << std::setprecision(2)
              << static_cast<double>(bestK) / hardwareThreads << '\n';
}

inline void growthFit(std::size_t kFrom, const LinearFit& fit) {
    std::cout << "FIT (K >= " << kFrom << "): t(K) = a + b*K, a = " << std::fixed
              << std::setprecision(4) << fit.intercept << " ms, b = " << std::setprecision(5)
              << fit.slope << " ms/thread, R^2 = " << std::setprecision(4) << fit.r2 << "\n\n";
}

}

