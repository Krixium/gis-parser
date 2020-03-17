#include "GeoFeature.h"

#include <sstream>

GeoFeature::GeoFeature(const std::vector<std::string>& tokens) {
    if (tokens.size() < 19) {
        return;
    }

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
}

// TODO: Change this to original format of GIS file
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