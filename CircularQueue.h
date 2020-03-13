#pragma once

#include <vector>

template <typename T>
class CircularQueue {
private:
    std::vector<T> data;

    int head;
    int tail;
    int maxSize;

public:
    CircularQueue<T>(const int size) 
        : head(0), tail(0), maxSize(size + 1), data() {
        this->data.resize(this->maxSize);
    }

    CircularQueue<T>(const CircularQueue<T>& other) {
        this = other;
    }

    CircularQueue<T>(CircularQueue<T>&& other) {
        this = std::move(other);
    }

    ~CircularQueue<T>() = default;

    CircularQueue<T>& operator=(const CircularQueue<T>& other) {
        if (&other != this) {
            this->head = other.head;
            this->tail = other.tail;
            this->maxSize = other.maxSize;

            this->data.reserve(this->maxSize);
            std::copy(other.data.begin(), other.data.end(), this->data.begin());
        }
        return *this;
    }

    CircularQueue<T>& operator=(CircularQueue<T>& other) {
        if (&other != this) {
            std::swap(this->head, other.head);
            std::swap(this->tail, other.tail);
            std::swap(this->maxSize, other.maxSize);
            this->data.swap(other.data);
        }
        return *this;

    }

    bool full() {
        if (this->head == 0) {
            return this->tail == this->maxSize - 1;
        } else {
            return this->tail == this->head - 1;
        }
    }

    bool empty() {
        return this->tail == this->head;
    }

    bool enque(const T item) {
        if (this->full()) {
            return false;
        }

        this->data[this->tail] = item;
        this->tail = (this->tail + 1) % this->maxSize;
        return true;
    }

    T deque() {
        if (empty()) {
            return (T)0;
        }

        T& tmp = this->data[this->head];
        this->head = (this->head + 1) % this->maxSize;
        return tmp;
    }
};
