#pragma once

#include <string>
#include <vector>

#include "Coord.h"

class GeoFeature {
    int offset = -1;
    int featureId;
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
    int elevationMeters;
    int elevationFeet;
    std::string mapName;
    std::string dateCreated;
    std::string dateEdited;

public:
    GeoFeature(const std::vector<std::string>& tokens);

    inline const int getId() const {
        return this->featureId;
    }

    inline const std::string& getName() const {
        return this->name;
    }

    inline const DecCoord getPrimCoordDec() const {
        if (!this->primDec.isValid()) {
            return DecCoord(this->primDms);
        } else {
            return this->primDec;
        }
    }

    std::string toString() const;

    inline friend std::ostream& operator<<(std::ostream& os, const GeoFeature& gf) {
        os << gf.toString();
        return os;
    }
};
