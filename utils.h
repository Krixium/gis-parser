#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#define DEBUG std::cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << std::endl

namespace utils {
    double timer(std::function<void()> callback);

    std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false);
};
