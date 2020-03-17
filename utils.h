#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <string>
#include <vector>

namespace utils {
    double timer(std::function<void()> callback);
    std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false);
};
