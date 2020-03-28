#pragma once

#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include "Coord.h"

class GeoFeature {
public:
    static const std::unordered_set<std::string> POP_TYPES;
    static const std::unordered_set<std::string> WATER_TYPES;
    static const std::unordered_set<std::string> STRUCTURE_TYPES;

private:
    std::size_t offset;
    int featureId = -1;
    std::string nameIndex;
    std::string name;
    std::string featureClass;
    std::string stateAlpha;
    std::string stateNumeric;
    std::string countyName;
    std::string countyNumeric;
    DmsCoord primDms;
    DecCoord primDec;
    DmsCoord srcDms;
    DecCoord srcDec;
    std::unique_ptr<int> elevationMeters;
    std::unique_ptr<int> elevationFeet;
    std::string mapName;
    std::string dateCreated;
    std::string dateEdited;

public:
    GeoFeature(const std::vector<std::string>& tokens, const std::size_t offset = -1);

    GeoFeature(const GeoFeature& other) { *this = other; }
    GeoFeature(GeoFeature&& other) { *this = std::move(other); }

    GeoFeature& operator=(const GeoFeature& other);
    GeoFeature& operator=(GeoFeature&& other);

    inline void setOffset(const std::size_t offset) { this->offset = offset; }

    inline const std::size_t getOffset() const { return this->offset; }
    inline const int getId() const { return this->featureId; }
    inline const std::string& getNameIndex() const { return this->nameIndex; }
    inline const std::string& getName() const { return this->name; }
    inline const std::string& getClass() const { return this->featureClass; }
    inline const std::string& getStateAlpha() const { return this->stateAlpha; }
    inline const std::string& getCountyName() const { return this->countyName; }
    inline const DmsCoord& getPrimCoordDms() const { return this->primDms; }
    inline const DecCoord& getPrimCoordDec() const { return this->primDec; }
    inline const DmsCoord& getSrcCoordDms() const { return this->srcDms; }
    inline const DecCoord& getSrcCoordDec() const { return this->srcDec; }

    std::string toString() const;

    std::string toLongFormatString() const;

    inline static bool nameAscending(const GeoFeature& a, const GeoFeature& b) {
        return a.name < b.name;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const GeoFeature& gf) {
        os << gf.toString();
        return os;
    }
};
