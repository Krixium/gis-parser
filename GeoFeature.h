#pragma once

#include <string>
#include <vector>

struct DmsCoord {
    std::string lat;
    std::string lng;

    inline std::string toString() const {
        if (lat == "" && lng == "") {
            return "DMS(N/A)";
        }

        return "DMS(" + lat + "," + lng + ")";
    }
};

struct DecCoord {
    double lat;
    double lng;

    inline std::string toString() const {
        if (lat == 0.0f && lng == 0.0f) {
            return "DEC(N/A)";
        }

        return "DEC(" + std::to_string(lat) + "," + std::to_string(lng) + ")";
    }
};

class GeoFeature {
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

    std::string toString() const;
};
