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

std::string GeoFeature::toString() const {
    std::ostringstream oss;

    oss << std::to_string(this->featureId) << " " << this->name << "\n";
    oss << "\tFeature class: " << this->featureClass << "\n";
    oss << "\tState: " << this->stateAlpha << "[" << this->stateNumeric << "]\n";
    oss << "\tCountry: " << this->countyName << "[" << this->countyNumeric << "]\n";
    oss << "\tPrimary Coordinates: " << this->primDms.toString() << " " << this->primDec.toString() << "\n";
    oss << "\tSource Coordinates: " << this->srcDms.toString() << " " << this->srcDec.toString() << "\n";
    oss << "\tElevation: " << std::to_string(this->elevationMeters) << " meters (" << std::to_string(this->elevationFeet) << " feet)\n";
    oss << "\tMap Name: " << this->mapName << "\n";
    oss << "\tDate Created: " << this->dateCreated << "\n";
    oss << "\tDate Edited: " << this->dateEdited << "\n";

    return oss.str();
}