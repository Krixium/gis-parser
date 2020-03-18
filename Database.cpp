#include "Database.h"

#include <set>

#include "utils.h"

Database::Database(const std::string& databaseFileName, const double x, const double y, const double halfWidth) {
    this->init(databaseFileName);
    this->setBounds(x, y, halfWidth);
}

Database::~Database() {
    std::cout << "Database::~Database()" << std::endl;
    if (this->storageFile.is_open()) {
        this->storageFile.close();
    }
}

void Database::setBounds(const double centerX, const double centerY, const double halfWidth) {
    this->coordIndex.setBound(centerX, centerY, halfWidth);
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
    if (!this->coordIndex.isUsable()) {
        return GeoFeature(std::vector<std::string>());
    }
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
    this->coordIndex.queryPoint(coord.getLat(), coord.getLng(), points);

    this->convertPointsToGeoFeatures(points, features);

    return features;
}

std::vector<GeoFeature> Database::searchByCoordinate(const DmsCoord& coord, double halfSize) {
    return this->searchByCoordinate(DecCoord(coord), halfSize);
}

std::vector<GeoFeature> Database::searchByCoordinate(const DecCoord& coord, double halfSize) {
    std::vector<GeoFeature> features;

    if (!this->coordIndex.isUsable()) {
        return features;
    }

    Quad range(coord.getLat(), coord.getLng(), halfSize);

    std::vector<const Point*> points;
    this->coordIndex.queryRange(range, points);

    this->convertPointsToGeoFeatures(points, features);

    return features;
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


