#pragma once

#include <chrono>
#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "GeoFeature.h"
#include "HashMap.h"
#include "QuadTree.h"

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
    std::fstream storageFile;

    std::list<GeoFeature> cache;

    HashMap<std::string, std::size_t> nameIndex;
    QuadTree coordIndex;

public:
    Database(const std::string& databaseFileName, const double x, const double y, const double halfWidth);
    ~Database();

    Database(const Database& other) = delete;
    Database(Database&& other) = delete;

    Database& operator=(const Database& other) = delete;
    Database& operator=(Database& other) = delete;

    void setBounds(const double centerX, const double centerY, const double halfWidth);

    bool storeToFile(const GeoFeature& entry);
    bool storeToFile(const std::string& line);

    GeoFeature searchByName(const std::string& name, const std::string& state);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord, double halfSize);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord, double halfSize);

private:
    void init(const std::string& databaseFile);

    void insertIntoHashMap(const GeoFeature& entry, const std::size_t offset);
    void insertIntoQuadTree(const DecCoord& coord, const std::size_t offset);

    GeoFeature getEntryFromDatabase(const std::size_t offset);

    void encache(const GeoFeature& entry);

    void convertPointsToGeoFeatures(const std::vector<const Point*>& points, std::vector<GeoFeature>& output);
};
