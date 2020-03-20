#include "Gis.h"

#include <memory>
#include <sstream>

#include "FileTokenizer.h"

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

    if (instruction == ScriptCommand::CMD_WORLD) {
        ret = this->createWorld(args[0], args[1], args[2], args[3]);
    } else if (instruction == ScriptCommand::CMD_IMPORT) {
        ret = this->importFeatures(args[0]);
    } else if (instruction == ScriptCommand::CMD_DEBUG) {
        ret = this->debug(args[0]);
    } else if (instruction == ScriptCommand::CMD_QUIT) {
        ret = true;
    } else if (instruction == ScriptCommand::CMD_WHAT_IS_AT) {
        ret = this->searchByCoordinate(args[0], args[1]);
    } else if (instruction == ScriptCommand::CMD_WHAT_IS) {
        ret = this->searchForName(args[0], args[1]);
    } else if (instruction == ScriptCommand::CMD_WHAT_IS_IN) {
        ret = this->searchByQuad(args[0], args[1], args[2], args[3]);
    } else if (instruction == ScriptCommand::COMMENT) {
        return true;
    }

    this->logString("-----------------------------------------------------------------");

    return true;
}

bool Gis::createWorld(const std::string& west, const std::string& east, const std::string& south, const std::string& north) {
    const double dWest = DecCoord::dmsToDecLng(west);
    const double dEast = DecCoord::dmsToDecLng(east);
    const double dSouth = DecCoord::dmsToDecLat(south);
    const double dNorth = DecCoord::dmsToDecLat(north);

    const double halfWidth = dEast - dWest;
    const double halfHeight = dNorth - dSouth;

    const double centerX = dWest + halfWidth;
    const double centerY = dSouth + halfHeight;

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
    const std::vector<GeoFeature>& features = this->db.searchByCoordinate(DmsCoord(lat, lng));

    for (const GeoFeature& feature : features) {
        std::ostringstream oss;
        oss << feature.getOffset() << " " << feature.getName() << " " << feature.getCountyName() << " " << feature.getStateAlpha();
        this->logString(oss.str());
    }

    return true;
}

bool Gis::importFeatures(const std::string& filename) {
    this->db.importData(filename);
    return true;
}

bool Gis::searchForName(const std::string& name, const std::string& state) {
    try {
        const std::vector<GeoFeature>& features = this->db.searchByName(name, state);

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
    const std::string& halfHeight, const std::string& halfWidth,
    const std::string& filter) {

    const std::vector<GeoFeature>& features = this->db.searchByCoordinate(DmsCoord(lat, lng), std::stod(halfWidth), std::stod(halfHeight));
    std::vector<GeoFeature> output;

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
        return false;
    }

    for (const GeoFeature& feature : features) {
        std::ostringstream oss;
        oss << std::to_string(feature.getOffset()) << " " << feature.getName() << " " << feature.getStateAlpha() << " " << feature.getPrimCoordDms();
        this->logString(oss.str());
    }

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
        oss << "Command " << this->commandsExecuted << ": " << command;
    }
    oss << std::endl;
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