#pragma once

#include <string>
#include <vector>

class DmsCoord {
private:
    std::string lat;
    std::string lng;

public:
    DmsCoord() {
        this->setLat("");
        this->setLng("");
    }

    DmsCoord(const std::string& lat, const std::string& lng) {
        this->setLat(lat);
        this->setLat(lng);
    }

    inline void setLat(const std::string& lat) {
        this->lat = lat;
    }

    inline void setLng(const std::string& lng) {
        this->lng = lng;
    }

    inline const std::string& getLat() const {
        return this->lat;
    }

    inline const std::string& getLng() const {
        return this->lng;
    }

    // TODO: Change this to original format of GIS file
    inline std::string toString() const {
        if (lat == "" && lng == "") {
            return "DMS(N/A)";
        }

        return "DMS(" + lat + "," + lng + ")";
    }

    inline friend std::ostream& operator<<(std::ostream& os, const DmsCoord& coord) {
        os << coord.toString();
        return os;
    }
};

class DecCoord {
private:
    double lat;
    double lng;
    
public:
    DecCoord() {
        this->setLat("");
        this->setLng("");
    }

    DecCoord(const std::string& lat, const std::string& lng) {
        this->setLat(lat);
        this->setLat(lng);
    }

    DecCoord(const DmsCoord& dms) {
        double degrees;
        double minutes;
        double seconds;
        int direction = 1;

        degrees = std::stod(dms.getLat().substr(0, 2));
        minutes = std::stod(dms.getLat().substr(2, 4)) / 60;
        seconds = std::stod(dms.getLat().substr(4, 6)) / 3600;
        direction = dms.getLat().back() == 'N' ? 1 : -1;
        this->lat = (degrees + minutes + seconds) * direction;

        degrees = std::stod(dms.getLng().substr(0, 3));
        minutes = std::stod(dms.getLng().substr(3, 5)) / 60;
        seconds = std::stod(dms.getLng().substr(5, 7)) / 3600;
        direction = dms.getLng().back() == 'E' ? 1 : -1;
        this->lat = (degrees + minutes + seconds) * direction;
    }

    inline void setLat(const std::string& lat) {
        this->lat = lat == "" ? 0 : std::stod(lat);
    }

    inline void setLng(const std::string& lng) {
        this->lng = lng == "" ? 0 : std::stod(lng);
    }

    inline void setLat(const double lat) {
        this->lat = lat;
    }

    inline void setLng(const double lng) {
        this->lng = lng;
    }

    inline const double getLat() const {
        return this->lat;
    }

    inline const double getLng() const {
        return this->lng;
    }

    // TODO: Change this to original format of GIS file
    inline std::string toString() const {
        if (lat == 0.0f && lng == 0.0f) {
            return "DEC(N/A)";
        }

        return "DEC(" + std::to_string(lat) + "," + std::to_string(lng) + ")";
    }

    inline friend std::ostream& operator<<(std::ostream& os, const DecCoord& coord) {
        os << coord.toString();
        return os;
    }
};

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

    std::string toString() const;

    inline friend std::ostream& operator<<(std::ostream& os, const GeoFeature& gf) {
        os << gf.toString();
        return os;
    }
};
