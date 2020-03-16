#pragma once

#include <chrono>
#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "GeoFeature.h"

class CacheEntry {
private:
    std::chrono::milliseconds timestamp;
    const GeoFeature& feature;

public:
    CacheEntry(const GeoFeature& gf) : feature(gf) {
        this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }

    inline std::chrono::milliseconds getTimestamp() {
        return this->timestamp;
    }
};

class Database {
private:
    static Database instance;

    std::fstream storageFile;

    std::list<GeoFeature> cache;

public:
    static void Init(const std::string& databaseFile) {
        instance.init(databaseFile);
    }

    static Database& Get() {
        return instance;
    }

    ~Database();

    GeoFeature searchByName(const std::string& name, const std::string& state);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord, double halfHeight, double halfWidth);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord, double halfHeight, double halfWidth);

    Database(const Database& other) = delete;
    Database(Database&& other) = delete;

    Database& operator=(const Database& other) = delete;
    Database& operator=(Database& other) = delete;

private:
    Database() = default;

    void init(const std::string& databaseFile);

    void encache(const GeoFeature& entry);

    std::size_t storeToFile(const GeoFeature& entry);
};
