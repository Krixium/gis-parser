#include "utils.h"

double utils::timer(std::function<void()> callback) {
    std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::high_resolution_clock::now();
    callback();
    std::chrono::time_point<std::chrono::steady_clock> endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> delta = endTime - startTime;
    return delta.count();
}

std::vector<std::string> utils::split(const std::string& str, const std::string& delims, const bool skipEmpty) {
    std::vector<std::string> output;

    auto first = str.begin();

    while (first != str.end()) {
        const auto second = std::find_first_of(first, str.end(), delims.begin(), delims.end());

        if (first != second || !skipEmpty) {
            output.emplace_back(first, second);
        }

        if (second == str.end()) {
            break;
        }

        first = std::next(second);
    }

    return output;
}
