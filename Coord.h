#pragma once

#include <string>
#include <sstream>

#include "utils.h"

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
        this->setLng(lng);
    }

    inline void setLat(const std::string& lat) {
        if (lat == "Unknown") {
            this->lat = "";
        } else {
            this->lat = lat;
        }
    }

    inline void setLng(const std::string& lng) {
        if (lng == "Unknown") {
            this->lat = "";
        } else {
            this->lng = lng;
        }
    }

    inline const std::string& getLat() const { return this->lat; }
    inline const std::string& getLng() const { return this->lng; }
    inline bool isValid() const { return this->lat != "" && this->lng != ""; }

    inline std::string toString() const {
        if (this->lat == "" && this->lng == "") {
            return "|";
        }

        return this->lat + "|" + this->lng;
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
        this->setLat(0);
        this->setLng(0);
    }

    DecCoord(const std::string& lat, const std::string& lng) {
        this->setLat(lat);
        this->setLat(lng);
    }

    DecCoord(const DmsCoord& dms) {
        this->lat = dmsToDecLat(dms.getLat());
        this->lng = dmsToDecLng(dms.getLng());
    }

    inline void setLat(const std::string& lat) {
        if (lat == "") {
            this->setLat(0);
        } else {
            this->setLat(std::stod(lat));
        }
    }

    inline void setLng(const std::string& lng) {
        if (lng == "") {
            this->setLng(0);
        } else {
            this->setLng(std::stod(lng));
        }
    }

    inline void setLat(const double lat) { this->lat = utils::round(lat); }
    inline void setLng(const double lng) { this->lng = utils::round(lng); }

    inline const double getLat() const { return this->lat; }
    inline const double getLng() const { return this->lng; }

    inline bool isValid() const { return this->lat != 0.0 && this->lng != 0.0; }

    inline std::string toString() const {
        if (this->lat == 0.0 && this->lng == 0.0) {
            return "|";
        }

        return std::to_string(lat) + "|" + std::to_string(lng);
    }

    inline friend std::ostream& operator<<(std::ostream& os, const DecCoord& coord) {
        os << coord.toString();
        return os;
    }

    inline static double dmsToDecLat(const std::string& lat) {
        double degrees;
        double minutes;
        double seconds;
        int direction = 1;

        if (lat == "") {
            return 0;
        } else {
            degrees = std::stod(lat.substr(0, 2));
            minutes = std::stod(lat.substr(2, 2)) / 60;
            seconds = std::stod(lat.substr(4, 2)) / 3600;
            direction = lat.back() == 'N' ? 1 : -1;
            return utils::round((degrees + minutes + seconds) * direction);
        }
    }

    inline static double dmsToDecLng(const std::string& lng) {
        double degrees;
        double minutes;
        double seconds;
        int direction = 1;

        if (lng == "") {
            return 0;
        } else {
            degrees = std::stod(lng.substr(0, 3));
            minutes = std::stod(lng.substr(3, 2)) / 60;
            seconds = std::stod(lng.substr(5, 2)) / 3600;
            direction = lng.back() == 'E' ? 1 : -1;
            return utils::round((degrees + minutes + seconds) * direction);
        }
    }

    inline static double secondsToDec(const std::string coord) {
        return utils::round(std::stod(coord) / 3600);
    }
};


