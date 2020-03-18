#include "Gis.h"

#include "FileTokenizer.h"

Gis::Gis(const std::string& databaseFile, const std::string& cmdScript, const std::string& logFile) {
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
    // iterate through every command and handle it

    // make sure that the WORLD command is called so that the database can be initialized
}

void Gis::logString(const std::string& msg) {

}

void Gis::logGeoFeature() {

}

void Gis::logHashMap() {

}

void Gis::logQuadTree() {

}