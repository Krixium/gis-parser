#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

template <typename T>
class HashMap {
private:
    static const std::size_t primes[];

    enum BucketStatus { EMPTY, OCCUPIED, DELETED };

    static const float rehashThreshold;

    std::size_t capacity = 0;
    std::size_t size = 0;
    std::size_t primeIndex = -1;

    std::vector<T> buckets;
    std::vector<BucketStatus> status;

    inline static std::size_t Hash(const T& key) {
        return std::hash<T>{}(key);
    }

    inline float getLoadRatio() { return this->size / this->capacity; }

    inline std::size_t calcNextIndex(const std::size_t hash, const std::size_t i) const {
        std::size_t n = hash + i;
        return ((n * n + n) / 2) % this->capacity;
    }

    std::size_t getNextSize() {
        static constexpr int primeSize = sizeof(primes) / sizeof(primes[0]);
        std::size_t nextSize = this->size;

        if (this->primeIndex == -1) {
            for (int i = 0; i < primeSize; i++) {
                if (primes[i] > nextSize) {
                    this->primeIndex = i;
                    nextSize = primes[i];
                    break;
                }
            }
        } else if (this->primeIndex >= primeSize) {
            nextSize *= 2;
        } else {
            this->primeIndex++;
            nextSize = primes[this->primeIndex];
        }
        return nextSize;
    }

    void expandAndRehash() {
        this->size = 0;
        this->capacity = this->getNextSize();
        std::vector<T> oldBuckets{ std::move(this->buckets) };
        std::vector<BucketStatus> oldStatus{ std::move(this->status) };

        this->buckets.resize(this->capacity);
        this->status.resize(this->capacity);

        for (int i = 0; i < oldBuckets.size(); i++) {
            if (oldStatus[i] == OCCUPIED) {
                this->insert(oldBuckets[i]);
            }
        }
    }

public:
    HashMap(const std::size_t n) : capacity(n) {
        if (n <= 1024) {
            this->capacity = 1024;
        } else {
            this->capacity = this->getNextSize();
        }
        this->buckets.resize(this->capacity);
        this->status.resize(this->capacity);
    }

    HashMap(const HashMap& other) {
        this = other;
    }

    HashMap(HashMap&& other) {
        this = std::move(other);
    }

    ~HashMap() = default;

    HashMap& operator=(const HashMap& other) {
        if (&other != this) {
            this->capacity = other.capacity;
            this->size = other.size;

            this->buckets.resize(this->capacity);
            this->status.resize(this->capacity);

            std::copy(other.buckets.begin(), other.buckets.end(), this->buckets.begin());
            std::copy(other.status.begin(), other.status.end(), this->buckets.begin());
        }

        return *this;
    }

    HashMap& operator=(HashMap&& other) {
        if (&other != this) {
            std::swap(this->capacity, other.capacity);
            std::swap(this->size, other.size);

            this->buckets.swap(other.buckets);
            this->status.swap(other.status);
        }

        return *this;
    }

    bool insert(const T& key) {
        std::set<unsigned long> probed;

        if (this->getLoadRatio() >= this->rehashThreshold) {
            this->expandAndRehash();
        }

        std::size_t h = Hash(key) % this->capacity;
        std::size_t i = 0;
        std::size_t hi = h + i;

        while (status[hi] == OCCUPIED) {
            if (buckets[hi] == key) {
                return false;
            }

            probed.insert(hi);

            i++;
            hi = this->calcNextIndex(h, i);

            if (probed.find(hi) != probed.end()) {
                this->expandAndRehash();
                probed.clear();
            }
        }

        buckets[hi] = key;
        status[hi] = OCCUPIED;
        this->size++;

        return true;
    }

    bool search(const T& key) const {
        std::set<unsigned long> probed;

        unsigned long h = Hash(key) % this->capacity;
        unsigned long i = 0;
        unsigned long hi = h + i;

        while (status[hi] != EMPTY) {
            if (status[hi] == OCCUPIED && buckets[hi] == key) {
                return true;
            }

            probed.insert(hi);

            i++;
            hi = this->calcNextIndex(h, i);

            if (probed.find(hi) != probed.end()) {
                return false;
            }
        }

        return false;
    }

    bool erase(const T& key) {
        std::set<unsigned> probed;
        unsigned long h = Hash(key) % this->capacity;
        unsigned long i = 0;
        unsigned long hi = h + i;

        while (status[hi] != EMPTY) {
            if (status[hi] == OCCUPIED && buckets[hi] == key) {
                status[hi] = DELETED;
                this->size--;
                return true;
            }

            probed.insert(hi);

            i++;
            hi = this->calcNextIndex(h, i);

            if (probed.find(hi) != probed.end()) {
                return false;
            }
        }

        return false;
    }
};

template <typename T>
const std::size_t HashMap<T>::primes[] = {
    53,        97,        193,       389,       769,       1543,     3079,
    6151,      12289,     24593,     49157,     98317,     196613,   393241,
    786433,    1572869,   3145739,   6291469,   12582917,  25165843, 50331653,
    100663319, 201326611, 402653189, 805306457, 1610612741
};

template <typename T>
const float HashMap<T>::rehashThreshold = 0.7;

