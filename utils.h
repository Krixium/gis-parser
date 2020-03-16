#pragma once

#include <algorithm>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false) {
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
