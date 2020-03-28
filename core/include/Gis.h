#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Database.h"
#include "GeoFeature.h"
#include "ScriptCommand.h"

class Gis {
private:
    std::vector<ScriptCommand> cmdSequence;

    Database db;

    std::fstream logFile;

    int commandsExecuted = -1;

public:
    Gis(const std::string& databaseFile, const std::string& cmdScript, const std::string& logFile);
    ~Gis() = default;

    Gis(const Gis& other) = delete;
    Gis(Gis&& other) = delete;

    Gis& operator=(const Gis& other) = delete;
    Gis& operator=(Gis&& other) = delete;

private:
    void openLogFile(const std::string& filename);
    void parseCmdScript(const std::string& script);

    void run();
    bool executeCommand(const ScriptCommand& cmd);

    bool createWorld(const std::string& west, const std::string& east, const std::string& south, const std::string& north);
    bool importFeatures(const std::string& filename);
    bool debug(const std::string& option);
    bool searchByCoordinate(const std::string& lat, const std::string& lng);
    bool searchForName(const std::string& name, const std::string& state);
    bool searchByQuad(const std::string& lat, const std::string& lng,
    const std::string& halfLat, const std::string& halfLng,
    const bool longFormat = false, const std::string& filter = "");

    void logString(const std::string& msg);
    void logCommand(const ScriptCommand& cmd);
    void logQuadTree();
    void logHashMap();
    void logPool();
};
