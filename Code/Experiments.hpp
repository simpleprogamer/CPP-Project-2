#pragma once
#include <algorithm>
#include <cstddef>
#include <execution>
#include <limits>
#include <set>
#include <vector>

#include "ChunkedAllOf.hpp"
#include "Report.hpp"
#include "SequenceFile.hpp"
#include "Stats.hpp"
#include "Stopwatch.hpp"

namespace experiments {


template <class Pred>
void runLibraryExperiment(const Sequence& data, const Pred& pred, int reps) {
    report::libraryHeader(reps);
    auto measure = [&](const char* label, auto&& algorithm) {
        bool result = false;
        const double ms = measureMedianMs(
            [&] {
                result = algorithm();
                resultSink = result;
            },
            reps);
        report::libraryRow(label, ms, result);
    };
    namespace ex = std::execution;
    measure("no policy", [&] { return std::all_of(data.begin(), data.end(), pred); });
    measure("seq", [&] { return std::all_of(ex::seq, data.begin(), data.end(), pred); });
    measure("par", [&] { return std::all_of(ex::par, data.begin(), data.end(), pred); });
    measure("par_unseq",
            [&] { return std::all_of(ex::par_unseq, data.begin(), data.end(), pred); });
#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201902L
    measure("unseq", [&] { return std::all_of(ex::unseq, data.begin(), data.end(), pred); });
#endif
}


inline std::vector<std::size_t> candidateThreadCounts(unsigned hardwareThreads) {
    std::set<std::size_t> ks{1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 128, 256, 512, 1024};
    ks.insert(hardwareThreads);
    ks.insert(2 * static_cast<std::size_t>(hardwareThreads));
    if (hardwareThreads >= 2) ks.insert(hardwareThreads / 2);
    return {ks.begin(), ks.end()};
}


template <class Pred>
void runOwnExperiment(const Sequence& data, const Pred& pred, int reps, unsigned hardwareThreads) {
    const double sequentialMs = measureMedianMs(
        [&] { resultSink = std::all_of(data.begin(), data.end(), pred); }, reps);
    report::ownHeader(sequentialMs, reps);

    std::size_t bestK = 1;
    double bestMs = std::numeric_limits<double>::max();
    std::vector<double> fitK, fitTime;
    const std::size_t fitFrom = 4 * static_cast<std::size_t>(hardwareThreads);

    for (const std::size_t k : candidateThreadCounts(hardwareThreads)) {
        const double ms = measureMedianMs(
            [&] { resultSink = chunkedAllOf(data.begin(), data.end(), pred, k); }, reps);
        report::ownRow(k, ms, sequentialMs / ms);
        if (ms < bestMs) {
            bestMs = ms;
            bestK = k;
        }
        if (k >= fitFrom) {
            fitK.push_back(static_cast<double>(k));
            fitTime.push_back(ms);
        }
    }
    report::ownSummary(bestK, bestMs, hardwareThreads);
    if (fitK.size() >= 2) report::growthFit(fitFrom, fitLinear(fitK, fitTime));
}

}  
