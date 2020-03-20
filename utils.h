#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <vector>


namespace utils {
    double timer(std::function<void()> callback);

    std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false);

    std::string generateIndent(const int size);

    double round(const double x);
};
