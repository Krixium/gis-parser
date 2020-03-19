#pragma once

#include <chrono>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "GeoFeature.h"
#include "HashMap.h"
#include "QuadTree.h"

class Database {
private:
    // Cache variables and classes
    class CacheEntry {
    private:
        std::chrono::milliseconds timestamp;
        const GeoFeature feature;

    public:
        CacheEntry(const GeoFeature& gf) : feature(gf) {
            this->updateTimestamp();
        }

        inline void updateTimestamp() {
            this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        }

        inline std::chrono::milliseconds getTimestamp() const { return this->timestamp; }
        inline const GeoFeature& getFeature() const { return this->feature; }

        inline std::string toString() const {
            std::ostringstream oss;
            oss << "{";
            oss << "timestamp:" << std::chrono::duration_cast<std::chrono::seconds>(this->timestamp).count() << ",";
            oss << "offset:" << this->feature.getOffset() << ",";
            oss << "nameIndex:" << this->feature.getNameIndex();
            oss << "}";
            return oss.str();
        }

        friend inline std::ostream& operator<<(std::ostream& os, const CacheEntry& ce) {
            os << ce.toString();
            return os;
        }
    };

    static constexpr int CACHE_SIZE = 15;
    static std::function<bool(const CacheEntry&, const CacheEntry&)> cacheOrder;

    std::list<CacheEntry> cache;

    // Database variables and classes
    std::fstream storageFile;

    HashMap<std::string, std::size_t> nameIndex;
    QuadTree coordIndex;

public:
    Database(const std::string& databaseFilename);
    ~Database();

    Database(const Database& other) = delete;
    Database(Database&& other) = delete;

    Database& operator=(const Database& other) = delete;
    Database& operator=(Database& other) = delete;

    void setBounds(const double centerX, const double centerY, const double halfWidth, const double halfHeight);
    void init(const std::string& databaseFile);
    void importData(const std::string& filename);

    bool storeToFile(const GeoFeature& entry);
    bool storeToFile(const std::string& line);

    GeoFeature searchByName(const std::string& name, const std::string& state);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord);

    std::vector<GeoFeature> searchByCoordinate(const DmsCoord& coord, const double halfWidth, const double halfHeight);
    std::vector<GeoFeature> searchByCoordinate(const DecCoord& coord, const double halfWidth, const double halfHeight);

    std::string quadTreeToString() const;
    std::string hashTableToString() const;
    std::string bufferPoolToString() const;

private:
    void insertIntoHashMap(const GeoFeature& entry, const std::size_t offset);
    void insertIntoQuadTree(const DecCoord& coord, const std::size_t offset);

    GeoFeature getEntryFromDatabase(const std::size_t offset);

    void encache(const GeoFeature& entry);

    void convertPointsToGeoFeatures(const std::vector<const Point*>& points, std::vector<GeoFeature>& output);
};
