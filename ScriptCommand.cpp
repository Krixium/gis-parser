#include "ScriptCommand.h"

#include <sstream>

const std::unordered_map<std::string, int> ScriptCommand::NUM_OF_ARGS = {
    {CMD_WORLD, 4},
    {CMD_IMPORT, 1},
    {CMD_DEBUG, 1},
    {CMD_QUIT, 0},
    {CMD_WHAT_IS_AT, 2},
    {CMD_WHAT_IS, 2},
    {CMD_WHAT_IS_IN, 4},
    {COMMENT, -1}
};

ScriptCommand::ScriptCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() < 1) {
        cmd = "";
        args.clear();
    } else if (NUM_OF_ARGS.find(tokens[0]) == NUM_OF_ARGS.end()) {
        cmd = "";
        args.clear();
    } else {
        cmd = tokens[0];

        if (cmd == COMMENT) {
            for (int i = 1; i < tokens.size(); i++) {
                args.push_back(tokens[i]);
            }
        } else {
            for (int i = 0; i < NUM_OF_ARGS.at(cmd); i++) {
                args.push_back(tokens[i + 1]);
            }
        }
    }
}

bool ScriptCommand::isValid() const {
    if (NUM_OF_ARGS.find(this->cmd) == NUM_OF_ARGS.end()) { return false; }

    if (this->cmd == COMMENT) {
        return true;
    } else if (this->cmd == CMD_WHAT_IS_IN) {
        if (this->args.size() == 4) { return true; }
        if (this->args.size() == 6 && this->args[4] == "-filter") { return true; }
        return false;
    } else {
        return this->args.size() == NUM_OF_ARGS.at(this->cmd);
    }
}

std::string ScriptCommand::toString() const {
    std::string delim = this->cmd == COMMENT ? " " : "\t";

    std::ostringstream oss;

    oss << this->cmd << delim;

    for (auto arg : this->args) {
        oss << arg << delim;
    }

    return oss.str();
}
