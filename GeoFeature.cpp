#include "GeoFeature.h"

#include <sstream>

GeoFeature::GeoFeature(const std::vector<std::string>& data) {
	if (data.size() < 19) {
		return;
	}

	this->featureId = std::stoi(data[0]);
	this->name = data[1];
	this->featureClass = data[2];
	this->stateAlpha = data[3];
	this->stateNumeric = data[4];
	this->countyName = data[5];
	this->countyNumeric = data[6];
	this->primDms.lat = data[7];
	this->primDms.lng = data[8];
	this->primDec.lat = data[9] == "" ? 0 : std::stod(data[9]);
	this->primDec.lng = data[10] == "" ? 0 : std::stod(data[10]);
	this->srcDms.lat = data[11];
	this->srcDms.lng = data[12];
	this->srcDec.lat = data[13] == "" ?  0 : std::stod(data[13]);
	this->srcDec.lng = data[14] == "" ? 0 : std::stod(data[14]);
	this->elevationMeters = data[15] == "" ? 0 : std::stoi(data[15]);
	this->elevationFeet = data[16] == "" ? 0 : std::stoi(data[16]);
	this->mapName = data[17];
	this->dateCreated = data[18];

	if (data.size() == 20) {
		this->dateEditted = data[19];
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
	oss << "\tDate Editted: ";
	oss << this->dateEditted;
	oss << "\n";

	return oss.str();
}