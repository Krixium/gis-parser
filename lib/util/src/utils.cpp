#include "utils.h"

#include <chrono>
#include <iostream>
#include <sstream>


double utils::timer(std::function<void()> callback) {
    auto startTime = std::chrono::high_resolution_clock::now();
    callback();
    auto endTime = std::chrono::high_resolution_clock::now();

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

std::string utils::generateIndent(const int size) {
    std::ostringstream oss;

    for (int i = 0; i < size; i++) {
        oss << "\t";
    }

    return oss.str();
}

template <typename T>
void utils::sort::quicksort(std::vector<T>& arr, int low, int high, bool(*f)(const T&, const T&)) {
    if (low < high) {
        int pi = partition(arr, low, high, f);

        quicksort(arr, low, pi - 1, f);
        quicksort(arr, pi + 1, high, f);
    }
}

template <typename T>
int utils::sort::partition(std::vector<T>& arr, int low, int high, bool(*f)(const T&, const T&)) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(&arr[i], &arr[j]);
        }
    }

    std::swap(&arr[i + 1], &arr[high]);

    return i + 1;
}
