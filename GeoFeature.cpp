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
    this->elevationMeters = tokens[15] == "" ? 0 : std::stoi(tokens[15]);
    this->elevationFeet = tokens[16] == "" ? 0 : std::stoi(tokens[16]);
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

std::string GeoFeature::toString() const {
    std::ostringstream oss;

    oss << std::to_string(this->featureId) << "|" << this->name << "|" << this->featureClass << "|";
    oss << this->stateAlpha << "|" << this->stateNumeric << "|";
    oss << this->countyName << "|" << this->countyNumeric << "|";
    oss << this->primDms << "|" << this->primDec << "|";
    oss << this->srcDms << "|" << this->srcDec << "|";
    oss << std::to_string(this->elevationMeters) << "|" << std::to_string(this->elevationFeet) << "|";
    oss << this->mapName << "|";
    oss << this->dateCreated << "|";
    oss << this->dateEdited << "|";

    return oss.str();
}