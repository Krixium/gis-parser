#include "Database.h"

#include "utils.h"

Database::~Database() {
    if (this->storageFile.is_open()) {
        this->storageFile.close();
    }
}

GeoFeature Database::searchByName(const std::string& name, const std::string& state) {
    return this->getEntryFromDatabase(this->nameIndex.get(name + "|" + state));
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

void Database::insertIntoHashMap(const GeoFeature& entry, const std::size_t offset) {
    this->nameIndex.insert(entry.getNameIndex(), offset);
}

void Database::insertIntoQuadTree(const DecCoord& coord, const std::size_t offset) {
    Point p(coord.getLat(), coord.getLng());
    p.indicies.push_back(offset);
    this->coordIndex.insert(p);
}

GeoFeature Database::getEntryFromDatabase(const std::size_t offset) {
    std::string line;

    this->storageFile.seekg(offset);
    std::getline(this->storageFile, line);

    return GeoFeature(utils::split(line, "|"));
}

void Database::encache(const GeoFeature& entry) {
    static const std::size_t cacheSize = 15;

    if (this->cache.size() >= 15) {
        this->cache.pop_back();
    }

    this->cache.emplace_front(entry);
}

void Database::storeToFile(const GeoFeature& entry) {
    std::size_t initPos = this->storageFile.tellg();
    this->storageFile << entry << std::endl;
    this->insertIntoHashMap(entry, initPos);
    this->insertIntoQuadTree(entry.getPrimCoordDec(), initPos);
}

void Database::storeToFile(const std::string& line) {
    this->storeToFile(GeoFeature(utils::split(line, "|")));
}
