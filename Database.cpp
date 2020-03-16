#include "Database.h"

Database::~Database() {
    if (this->storageFile.is_open()) {
        this->storageFile.close();
    }
}

GeoFeature Database::searchByName(const std::string& name, const std::string& state) {
    // TODO: Implement
    return GeoFeature(std::vector<std::string>{});
}

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord) {
    // TODO: Implement
    return std::vector<GeoFeature>{};
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord) {
    // TODO: Implement
    return std::vector<GeoFeature>{};
}

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord, double halfHeight, double halfWidth) {
    // TODO: Implement
    return std::vector<GeoFeature>{};
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord, double halfHeight, double halfWidth) {
    // TODO: Implement
    return std::vector<GeoFeature>{};
}

void Database::init(const std::string& databaseFile) {
    this->storageFile.open(databaseFile, std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void Database::encache(const GeoFeature& entry) {
    static const std::size_t cacheSize = 15;

    if (this->cache.size() >= 15) {
        this->cache.pop_back();
    }

    this->cache.emplace_front(entry);
}

std::size_t Database::storeToFile(const GeoFeature& entry) {
    // TODO: Implement
    return -1;
}
