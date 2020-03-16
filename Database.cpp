#include "Database.h"

#include "utils.h"

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

void Database::storeToFile(const GeoFeature& entry) {
    // save the offset before insert
    std::size_t initPos = this->storageFile.tellg();

    // write the line into database file
    this->storageFile << entry << std::endl;

    // save the name index to map
    this->nameIndex.insert(getNameIndex(std::to_string(entry.getId()), entry.getName()), initPos);

    // TODO: add it to the coordinate quad tree
}

void Database::storeToFile(const std::string& line) {
    // save the offset before insert
    std::size_t initPos = this->storageFile.tellg();
    std::size_t bytesWritten = 0;

    // write the line into database file
    this->storageFile.write(line.data(), line.length());
    this->storageFile << std::endl;

    // save the name index to map
    std::vector<std::string> tokens = split(line, "|", true);
    this->nameIndex.insert(getNameIndex(tokens[0], tokens[1]), initPos);

    // TODO: add it into the coordinate quad tree
}
