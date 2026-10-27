
#include <iostream>
#include <random>
#include <string>

#include "Config.hpp"
#include "SequenceFile.hpp"

static Sequence generateSequence(std::size_t n, std::mt19937& engine) {
    std::uniform_int_distribution<int> dist(config::kMinValue, config::kMaxValue);
    Sequence data(n);
    for (int& x : data) x = dist(engine);
    return data;
}

int main(int argc, char** argv) {
    const std::string dir = argc > 1 ? argv[1] : "data";
    std::filesystem::create_directories(dir);
    std::mt19937 engine(config::kSeed);
    for (const std::size_t n : config::kSequenceSizes) {
        const std::string path = dir + "/" + config::sequenceFileName(n);
        writeSequence(path, generateSequence(n, engine));
        std::cout << "written " << path << " (" << n << " elements)\n";
    }
}
