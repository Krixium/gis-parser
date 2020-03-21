#include "Database.h"

#include <set>

#include "utils.h"

std::function<bool(const Database::CacheEntry&, const Database::CacheEntry&)> Database::cacheOrder = [](const CacheEntry& a, const CacheEntry& b) {
    return a.getTimestamp() > b.getTimestamp();
};

Database::Database(const std::string& databaseFilename) {
    this->init(databaseFilename);
}

Database::~Database() {
    if (this->storageFile.is_open()) {
        this->storageFile.close();
    }
}

void Database::setBounds(const float centerX, const float centerY, const float halfWidth, const float halfHeight) {
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

std::vector<GeoFeature> Database::searchByName(const std::string& name, const std::string& state) {
    return this->getsEntryFromDatabase(this->nameIndex.get(name + "|" + state));
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

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord, const float halfWidth, const float halfHeight) {
    return this->searchByCoordinate(DecCoord(coord), halfWidth, halfHeight);
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord, const float halfWidth, const float halfHeight) {
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

    oss << "MRU" << std::endl;
    for (const CacheEntry& ce : this->cache) {
        oss << i++ << ": " << ce << std::endl;
    }
    oss << "LRU" << std::endl;

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

std::vector<GeoFeature> Database::getsEntryFromDatabase(const std::vector<std::size_t>& offsets) {
    bool inCache = false;
    std::string line;
    std::vector<GeoFeature> features;

    for (auto offset : offsets) {
        // check cache
        for (auto it = this->cache.begin(); it != this->cache.end(); ++it) {
            if (it->getFeature().getOffset() == offset) {
                features.push_back(it->getFeature());
                it->updateTimestamp();
                this->cache.sort(cacheOrder);
                inCache = true;
            }
        }

        if (inCache) {
            continue;
        }

        // not in cache, retrieve from file
        this->storageFile.seekg(offset);
        std::getline(this->storageFile, line);

        const GeoFeature result(utils::split(line, "|"), offset);
        this->encache(result);

        features.push_back(result);
    }

    return features;
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

    std::vector<GeoFeature> features = this->getsEntryFromDatabase(std::vector<std::size_t>(offsets.begin(), offsets.end()));

    output.clear();
    output.insert(output.end(), features.begin(), features.end());
}

