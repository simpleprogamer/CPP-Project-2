// ?????????? ?????? 2, ??????? 1: all_of.
// ??????????: MSVC v143 (Visual Studio 2022 Community), /std:c++20.
// ????????????: Release (/O2) ?? ReleaseNoOpt (/Od).
// ??????: Benchmark.exe <????_?_??????> <?????_??????>   (???? ??????? DataGenerator.exe)
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>

#include "Config.hpp"
#include "Experiments.hpp"
#include "Predicates.hpp"
#include "Report.hpp"
#include "SelfTest.hpp"
#include "SequenceFile.hpp"

template <class Pred>
static void investigate(const Sequence& data, const std::string& file, unsigned hardwareThreads) {
    const Pred pred;
    const int reps = config::repetitionsFor(data.size());
    report::sectionHeader(data.size(), file, Pred::kName, Pred::kDescription);
    experiments::runLibraryExperiment(data, pred, reps);
    experiments::runOwnExperiment(data, pred, reps, hardwareThreads);
}

int main(int argc, char** argv) {
    const std::string dir = argc > 1 ? argv[1] : "data";
    const std::string label = argc > 2 ? argv[2] : "unspecified";
    const unsigned hardwareThreads = std::max(1u, std::thread::hardware_concurrency());

    report::banner(label, hardwareThreads);
    if (!runSelfTest()) return 1;

    for (const std::size_t n : config::kSequenceSizes) {
        const std::string file = config::sequenceFileName(n);
        const Sequence data = readSequence(dir + "/" + file);
        investigate<IsPositive>(data, file, hardwareThreads);
        investigate<IsPositiveExpensive>(data, file, hardwareThreads);
    }
}
