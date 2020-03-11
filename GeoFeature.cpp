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
    this->primDms.lat = tokens[7];
    this->primDms.lng = tokens[8];
    this->primDec.lat = tokens[9] == "" ? 0 : std::stod(tokens[9]);
    this->primDec.lng = tokens[10] == "" ? 0 : std::stod(tokens[10]);
    this->srcDms.lat = tokens[11];
    this->srcDms.lng = tokens[12];
    this->srcDec.lat = tokens[13] == "" ? 0 : std::stod(tokens[13]);
    this->srcDec.lng = tokens[14] == "" ? 0 : std::stod(tokens[14]);
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

    oss << std::to_string(this->featureId);
    oss << " ";
    oss << this->name;
    oss << "\n";
    oss << "\tFeature class: ";
    oss << this->featureClass;
    oss << "\n";
    oss << "\tState: ";
    oss << this->stateAlpha;
    oss << "[";
    oss << this->stateNumeric;
    oss << "]\n";
    oss << "\tCountry: ";
    oss << this->countyName;
    oss << "[";
    oss << this->countyNumeric;
    oss << "]\n";
    oss << "\tPrimary Coordinates: ";
    oss << this->primDms.toString();
    oss << " ";
    oss << this->primDec.toString();
    oss << "\n";
    oss << "\tSource Coordinates: ";
    oss << this->srcDms.toString();
    oss << " ";
    oss << this->srcDec.toString();
    oss << "\n";
    oss << "\tElevation: ";
    oss << std::to_string(this->elevationMeters);
    oss << " meters (";
    oss << std::to_string(this->elevationFeet);
    oss << " feet)\n";
    oss << "\tMap Name: ";
    oss << this->mapName;
    oss << "\n";
    oss << "\tDate Created: ";
    oss << this->dateCreated;
    oss << "\n";
    oss << "\tDate Edited: ";
    oss << this->dateEdited;
    oss << "\n";

    return oss.str();
}