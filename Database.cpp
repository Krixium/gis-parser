#include "Database.h"

#include <set>

#include "utils.h"

std::function<bool(const Database::CacheEntry&, const Database::CacheEntry&)> Database::cacheOrder = [](const CacheEntry& a, const CacheEntry& b) {
    return a.getTimestamp() < b.getTimestamp();
};

Database::Database(const std::string& databaseFilename) {
    this->init(databaseFilename);
}

Database::~Database() {
    if (this->storageFile.is_open()) {
        this->storageFile.close();
    }
}

void Database::setBounds(const double centerX, const double centerY, const double halfWidth, const double halfHeight) {
    this->coordIndex.setBound(centerX, centerY, halfWidth, halfHeight);
}

void Database::init(const std::string& databaseFile) {
    if (databaseFile == "") {
        return;
    }

    if (this->storageFile.is_open()) {
        this->storageFile.close();
        this->cache.clear();
        this->nameIndex.clear();
        this->coordIndex.clear();
    }

    this->storageFile.open(databaseFile, std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void Database::importData(const std::string& filename) {
    std::string line;
    std::ifstream file;

    file.open(filename);

    // ignore the header
    std::getline(file, line);

    while (!file.eof()) {
        std::getline(file, line);
        if (line == "") { continue; }
        this->storeToFile(line);
    }
}

bool Database::storeToFile(const GeoFeature& entry) {
    if (!this->coordIndex.isUsable()) {
        return false;
    }
    std::size_t initPos = this->storageFile.tellg();
    this->storageFile << entry << std::endl;
    this->insertIntoHashMap(entry, initPos);
    this->insertIntoQuadTree(entry.getPrimCoordDec(), initPos);
    return true;
}

bool Database::storeToFile(const std::string& line) {
    return this->storeToFile(GeoFeature(utils::split(line, "|")));
}

GeoFeature Database::searchByName(const std::string& name, const std::string& state) {
    return this->getEntryFromDatabase(this->nameIndex.get(name + "|" + state));
}

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord) {
    return this->searchByCoordinate(DecCoord(coord));
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord) {
    std::vector<GeoFeature> features;

    if (!this->coordIndex.isUsable()) {
        return features;
    }

    std::vector<const Point*> points;
    this->coordIndex.queryPoint(coord.getLng(), coord.getLat(), points);

    this->convertPointsToGeoFeatures(points, features);

    return features;
}

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord, const double halfWidth, const double halfHeight) {
    return this->searchByCoordinate(DecCoord(coord), halfWidth, halfHeight);
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord, const double halfWidth, const double halfHeight) {
    std::vector<GeoFeature> features;

    if (!this->coordIndex.isUsable()) {
        return features;
    }

    Quad range(coord.getLng(), coord.getLat(), halfWidth, halfHeight);

    std::vector<const Point*> points;
    this->coordIndex.queryRange(range, points);

    this->convertPointsToGeoFeatures(points, features);

    return features;
}

std::string Database::quadTreeToString() const {
    return this->coordIndex.toString();
}

std::string Database::hashTableToString() const {
    return this->nameIndex.toString();
}

std::string Database::bufferPoolToString() const {
    int i = 1;
    std::ostringstream oss;
    oss << "Buffer Pool:" << std::endl;

    for (const CacheEntry& ce : this->cache) {
        oss << i << ": " << ce << std::endl;
    }

    return oss.str();
}

void Database::insertIntoHashMap(const GeoFeature& entry, const std::size_t offset) {
    this->nameIndex.insert(entry.getNameIndex(), offset);
}

void Database::insertIntoQuadTree(const DecCoord& coord, const std::size_t offset) {
    Point p(coord.getLng(), coord.getLat());
    p.indicies.push_back(offset);
    this->coordIndex.insert(p);
}

GeoFeature Database::getEntryFromDatabase(const std::size_t offset) {
    // check the cache
    for (auto it = this->cache.begin(); it != this->cache.end(); ++it) {
        if (it->getFeature().getOffset() == offset) {
            const GeoFeature& result = it->getFeature();

            it->updateTimestamp();
            this->cache.sort(cacheOrder);

            return result;
        }
    }

    // not in cache, retrieve from file
    std::string line;
    this->storageFile.seekg(offset);
    std::getline(this->storageFile, line);

    // encache result before returning
    const GeoFeature result(utils::split(line, "|"), offset);
    this->encache(result);

    return result;
}

void Database::encache(const GeoFeature& entry) {
    if (this->cache.size() >= CACHE_SIZE) {
        this->cache.pop_back();
    }

    this->cache.emplace_front(entry);
    this->cache.sort(cacheOrder);
}

void Database::convertPointsToGeoFeatures(const std::vector<const Point*>& points, std::vector<GeoFeature>& output) {
    std::set<std::size_t> offsets;
    for (const Point* p : points) {
        for (const std::size_t i : p->indicies) {
            offsets.insert(i);
        }
    }

    for (const std::size_t offset : offsets) {
        GeoFeature& feature = this->getEntryFromDatabase(offset);
        feature.setOffset(offset);
        output.push_back(feature);
    }
}

