#include "GeoFeature.h"

#include <sstream>

const std::unordered_set<std::string> GeoFeature::POP_TYPES = {
    "Populated Place"
};

const std::unordered_set<std::string> GeoFeature::WATER_TYPES = {
    "Arroyo", "Basin", "Bay", "Bend", "Canal", "Channel", "Falls",
    "Glacier", "Gut", "Harbor", "Lake", "Rapids", "Reservoir",
    "Sea", "Spring", "Stream", "Swamp", "Well"
};

const std::unordered_set<std::string> GeoFeature::STRUCTURE_TYPES = {
    "Airport", "Bridge", "Building", "Church", "Dam", "Hospital",
    "Levee", "Park", "Post Office", "School", "Tower", "Tunnel"
};

GeoFeature::GeoFeature(const std::vector<std::string>& tokens, const std::size_t offset) {
    if (tokens.size() < 19) {
        return;
    }

    this->offset = offset;
    this->featureId = std::stoi(tokens[0]);
    this->name = tokens[1];
    this->featureClass = tokens[2];
    this->stateAlpha = tokens[3];
    this->stateNumeric = tokens[4];
    this->countyName = tokens[5];
    this->countyNumeric = tokens[6];
    this->primDms.setLat(tokens[7]);
    this->primDms.setLng(tokens[8]);
    this->primDec.setLat(tokens[9]);
    this->primDec.setLng(tokens[10]);
    this->srcDms.setLat(tokens[11]);
    this->srcDms.setLng(tokens[12]);
    this->srcDec.setLat(tokens[13]);
    this->srcDec.setLng(tokens[14]);

    if (tokens[15] == "") {
        this->elevationMeters.reset(nullptr);
    } else {
        this->elevationMeters = std::make_unique<int>(std::stoi(tokens[15]));
    }

    if (tokens[16] == "") {
        this->elevationFeet.reset(nullptr);
    } else {
        this->elevationFeet = std::make_unique<int>(std::stoi(tokens[16]));
    }

    this->mapName = tokens[17];
    this->dateCreated = tokens[18];

    if (tokens.size() == 20) {
        this->dateEdited = tokens[19];
    }

    // generate name index
    std::ostringstream oss;
    oss << this->name << "|" << this->stateAlpha;
    this->nameIndex = oss.str();

    // create valid DEC coordinates
    if (!this->primDec.isValid()) {
        this->primDec = DecCoord(this->primDms);
    }

    if (!this->srcDec.isValid()) {
        this->srcDec = DecCoord(this->srcDms);
    }
}

GeoFeature& GeoFeature::operator=(const GeoFeature& other) {
    if (this != &other) {
        this->offset = other.offset;
        this->featureId = other.featureId;
        this->nameIndex = other.nameIndex;
        this->name = other.name;
        this->featureClass = other.featureClass;
        this->stateAlpha = other.stateAlpha;
        this->stateNumeric = other.stateNumeric;
        this->countyName = other.countyName;
        this->countyNumeric = other.countyNumeric;
        this->primDms = other.primDms;
        this->primDec = other.primDec;
        this->srcDms = other.srcDms;
        this->srcDec = other.srcDec;
        if (other.elevationMeters) {
            this->elevationMeters = std::make_unique<int>(*other.elevationMeters);
        }
        if (other.elevationFeet) {
            this->elevationFeet = std::make_unique<int>(*other.elevationFeet);
        }
        this->mapName = other.mapName;
        this->dateCreated = other.dateCreated;
        this->dateEdited = other.dateEdited;
    }
    return *this;
}

GeoFeature& GeoFeature::operator=(GeoFeature&& other) {
    if (this != &other) {
        std::swap(this->offset, other.offset);
        std::swap(this->featureId, other.featureId);
        std::swap(this->nameIndex, other.nameIndex);
        std::swap(this->name, other.name);
        std::swap(this->featureClass, other.featureClass);
        std::swap(this->stateAlpha, other.stateAlpha);
        std::swap(this->stateNumeric, other.stateNumeric);
        std::swap(this->countyName, other.countyName);
        std::swap(this->countyNumeric, other.countyNumeric);
        std::swap(this->primDms, other.primDms);
        std::swap(this->primDec, other.primDec);
        std::swap(this->srcDms, other.srcDms);
        std::swap(this->srcDec, other.srcDec);
        std::swap(this->elevationMeters, other.elevationMeters);
        std::swap(this->elevationFeet, other.elevationFeet);
        std::swap(this->mapName, other.mapName);
        std::swap(this->dateCreated, other.dateCreated);
        std::swap(this->dateEdited, other.dateEdited);
    }
    return *this;
}

std::string GeoFeature::toString() const {
    std::ostringstream oss;

    oss << std::to_string(this->featureId) << "|" << this->name << "|" << this->featureClass << "|";
    oss << this->stateAlpha << "|" << this->stateNumeric << "|";
    oss << this->countyName << "|" << this->countyNumeric << "|";
    oss << this->primDms << "|" << this->primDec << "|";
    oss << this->srcDms << "|" << this->srcDec << "|";
    if (this->elevationMeters != nullptr) { oss << std::to_string(*this->elevationMeters); }
    oss << "|";
    if (this->elevationFeet != nullptr) { oss << std::to_string(*this->elevationFeet); }
    oss << "|";
    oss << this->mapName << "|";
    oss << this->dateCreated << "|";
    oss << this->dateEdited << "|";

    return oss.str();
}

std::string GeoFeature::toLongFormatString() const {
    std::ostringstream oss;

    if (this->featureId != -1) {
        oss << "Feature ID: " << this->featureId << std::endl;
    }

    if (this->name != "") {
        oss << "Feature Name: " << this->name << std::endl;
    }

    if (this->featureClass != "") {
        oss << "Feature Class: " << this->featureClass << std::endl;
    }

    if (this->stateAlpha != "") {
        oss << "State Alpha: " << this->stateAlpha << std::endl;
    }

    if (this->stateNumeric != "") {
        oss << "State Numeric: " << this->stateNumeric << std::endl;
    }

    if (this->countyName != "") {
        oss << "County Name: " << this->countyName << std::endl;
    }

    if (this->countyNumeric != "") {
        oss << "County Numeric: " << this->countyNumeric << std::endl;
    }

    if (this->primDms.isValid()) {
        oss << "Primary Coordinate(DMS): " << this->primDms << std::endl;
    }

    if (this->primDec.isValid()) {
        oss << "Primary Coordinate(DEC): " << this->primDec << std::endl;
    }

    if (this->srcDms.isValid()) {
        oss << "Source Coordinate(DMS): " << this->srcDms << std::endl;
    }

    if (this->srcDec.isValid()) {
        oss << "Source Coordinate(DEC): " << this->srcDec << std::endl;
    }

    if (this->elevationMeters != nullptr) {
        oss << "Elevation in Meters: " << *this->elevationMeters << std::endl;
    }

    if (this->elevationFeet != nullptr) {
        oss << "Elevation in Feet: " << *this->elevationFeet << std::endl;
    }

    if (this->mapName != "") {
        oss << "Map Name: " << this->mapName << std::endl;
    }

    if (this->dateCreated != "") {
        oss << "Date Created: " << this->dateCreated << std::endl;
    }

    if (this->dateEdited != "") {
        oss << "Date Edited: " << this->dateEdited << std::endl;
    }

    return oss.str();
}
