#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <vector>


namespace utils {
    double timer(std::function<void()> callback);

    std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false);

    std::string generateIndent(const int size);

    template <typename T> 
    inline void sortVector(std::vector<T>& vec, bool(*f)(const T&, const T&)) {
        std::sort(vec.begin(), vec.end(), f);
    }

    namespace sort {
        template <typename T>
        void quicksort(std::vector<T>& arr, int low, int high, bool(*f)(const T&, const T&));

        template <typename T>
        int partition(std::vector<T>& arr, int low, int high, bool (*f)(const T&, const T&));
    }
};
