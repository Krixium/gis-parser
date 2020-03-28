#pragma once

#include <functional>
#include <string>
#include <vector>


namespace utils {
    double timer(std::function<void()> callback);

    std::vector<std::string> split(const std::string& str, const std::string& delims, const bool skipEmpty = false);

    std::string generateIndent(const int size);

    template <typename T>
    void sortVector(std::vector<T>& vec, bool(*f)(const T&, const T&));

    namespace sort {
        template <typename T>
        void quicksort(std::vector<T>& arr, int low, int high, bool(*f)(const T&, const T&));

        template <typename T>
        int partition(std::vector<T>& arr, int low, int high, bool (*f)(const T&, const T&));
    }
};

template <typename T>
void utils::sortVector(std::vector<T>& vec, bool(*f)(const T&, const T&)) {
    sort::quicksort(vec, 0, vec.size() - 1, f);
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
    const T& pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (f(arr[j], pivot)) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);

    return i + 1;
}


