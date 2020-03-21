#include "Gis.h"

#include <memory>
#include <sstream>

#include "FileTokenizer.h"
#include "utils.h"

Gis::Gis(const std::string& databaseFile, const std::string& cmdScript, const std::string& logFile) : db(databaseFile) {
    this->openLogFile(logFile);
    this->parseCmdScript(cmdScript);
    this->run();
}

void Gis::openLogFile(const std::string& filename) {
    if (this->logFile.is_open()) {
        this->logFile.close();
    }

    this->logFile.open(filename, std::fstream::out | std::fstream::trunc);
}

void Gis::parseCmdScript(const std::string& script) {
    FileTokenizer cmdFile(script, "\t");

    while (!cmdFile.eof()) {
        ScriptCommand cmd(cmdFile.getNextLineAsTokens());

        if (cmd.isValid()) {
            this->cmdSequence.push_back(cmd);
        }
    }
}

void Gis::run() {
    for (const ScriptCommand& cmd : this->cmdSequence) {
        this->executeCommand(cmd);
    }
}

bool Gis::executeCommand(const ScriptCommand& cmd) {
    bool ret = false;
    if (!cmd.isValid()) { return ret; }

    const std::string& instruction = cmd.getCmd();
    const std::vector<std::string>& args = cmd.getArgs();

    this->logCommand(cmd);

    if (instruction == ScriptCommand::COMMENT) {
        return true;
    }

    if (instruction == ScriptCommand::CMD_QUIT) {
        return true;
    }

    float time = utils::timer([&]() {
        if (instruction == ScriptCommand::CMD_WORLD) {
            ret = this->createWorld(args[0], args[1], args[2], args[3]);
        } else if (instruction == ScriptCommand::CMD_IMPORT) {
            ret = this->importFeatures(args[0]);
        } else if (instruction == ScriptCommand::CMD_DEBUG) {
            ret = this->debug(args[0]);
        } else if (instruction == ScriptCommand::CMD_WHAT_IS_AT) {
            ret = this->searchByCoordinate(args[0], args[1]);
        } else if (instruction == ScriptCommand::CMD_WHAT_IS) {
            ret = this->searchForName(args[0], args[1]);
        } else if (instruction == ScriptCommand::CMD_WHAT_IS_IN) {
            // TOOD: implement -long
            if (args[0] == "-long") {
                this->searchByQuad(args[1], args[2], args[3], args[4], true);
            } else if (args[0] == "-filter") {
                this->searchByQuad(args[2], args[3], args[4], args[5], false, args[1]);
            } else {
                ret = this->searchByQuad(args[0], args[1], args[2], args[3]);
            }
        } else {
            // skip invalid command
        }
    });

    this->logString("Time elapsed: " + std::to_string(time) + "s");
    this->logString("-----------------------------------------------------------------");

    return true;
}

bool Gis::createWorld(const std::string& west, const std::string& east, const std::string& south, const std::string& north) {
    // TODO: coordinates
    const float dWest = DecCoord::dmsToDecLng(west);
    const float dEast = DecCoord::dmsToDecLng(east);
    const float dSouth = DecCoord::dmsToDecLat(south);
    const float dNorth = DecCoord::dmsToDecLat(north);

    const float halfWidth = dEast - dWest;
    const float halfHeight = dNorth - dSouth;

    const float centerX = dWest + halfWidth;
    const float centerY = dSouth + halfHeight;

    this->db.setBounds(centerX, centerY, halfWidth, halfHeight);

    return true;
}

bool Gis::debug(const std::string& option) {
    if (option == "quad") {
        this->logQuadTree();
    } else if (option == "hash") {
        this->logHashMap();
    } else if (option == "pool") {
        this->logPool();
    } else {
        return false;
    }
    return true;
}

bool Gis::searchByCoordinate(const std::string& lat, const std::string& lng) {
    // TODO: coordinates
    std::vector<GeoFeature>& features = this->db.searchByCoordinate(DmsCoord(lat, lng));

    utils::sortVector(features, GeoFeature::nameAscending);

    std::ostringstream oss;
    for (const GeoFeature& feature : features) {
        oss << feature.getOffset() << " " << feature.getName() << " " << feature.getCountyName() << " " << feature.getStateAlpha() << std::endl;
    }
    this->logString(oss.str());

    return true;
}

bool Gis::importFeatures(const std::string& filename) {
    this->db.importData(filename);
    return true;
}

bool Gis::searchForName(const std::string& name, const std::string& state) {
    try {
        std::vector<GeoFeature>& features = this->db.searchByName(name, state);

        utils::sortVector(features, GeoFeature::nameAscending);

        std::ostringstream oss;
        for (const GeoFeature& feature : features) {
            oss << std::to_string(feature.getOffset()) << " " << feature.getCountyName() << " " << feature.getPrimCoordDms() << std::endl;
        }

        this->logString(oss.str());

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Gis::searchByQuad(const std::string& lat, const std::string& lng,
    const std::string& halfLat, const std::string& halfLng,
    const bool longFormat, const std::string& filter) {

    // TODO: coordinates

    std::vector<GeoFeature> output;
    const std::vector<GeoFeature>& features =
        this->db.searchByCoordinate(DmsCoord(lat, lng), DecCoord::secondsToDec(halfLng), DecCoord::secondsToDec(halfLat));

    if (filter == "pop") {
        std::copy_if(features.begin(), features.end(), std::back_inserter(output), [](const GeoFeature& f) {
            return GeoFeature::POP_TYPES.find(f.getClass()) != GeoFeature::POP_TYPES.end();
        });
    } else if (filter == "water") {
        std::copy_if(features.begin(), features.end(), std::back_inserter(output), [](const GeoFeature& f) {
            return GeoFeature::WATER_TYPES.find(f.getClass()) != GeoFeature::WATER_TYPES.end();
        });
    } else if (filter == "structure") {
        std::copy_if(features.begin(), features.end(), std::back_inserter(output), [](const GeoFeature& f) {
            return GeoFeature::STRUCTURE_TYPES.find(f.getClass()) != GeoFeature::STRUCTURE_TYPES.end();
        });
    } else {
        output.insert(output.end(), features.begin(), features.end());
    }

    utils::sortVector(output, GeoFeature::nameAscending);

    std::ostringstream oss;
    for (const GeoFeature& feature : output) {
        if (longFormat) {
            oss << feature.toLongFormatString() << std::endl;
        } else {
            oss << std::to_string(feature.getOffset()) << " "
                << feature.getName() << " "
                << feature.getStateAlpha() << " "
                << feature.getPrimCoordDms() << std::endl;
        }
    }
    this->logString(oss.str());

    return true;
}

void Gis::logString(const std::string& msg) {
    this->logFile << msg << std::endl;
}

void Gis::logCommand(const ScriptCommand& command) {
    std::ostringstream oss;
    if (command.getCmd() == ScriptCommand::COMMENT) {
        oss << command;
    } else {
        this->commandsExecuted++;
        oss << "Command " << this->commandsExecuted << ": " << command << std::endl;
    }
    this->logString(oss.str());
}

void Gis::logQuadTree() {
    this->logString(this->db.quadTreeToString());
}

void Gis::logHashMap() {
    this->logString(this->db.hashTableToString());
}

void Gis::logPool() {
    this->logString(this->db.bufferPoolToString());
}