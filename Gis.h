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

    std::unique_ptr<Database> db;

    std::fstream logFile;

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

    void logString(const std::string& msg);
    void logGeoFeature();
    void logHashMap();
    void logQuadTree();
};
