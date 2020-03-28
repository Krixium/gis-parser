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
    {COMMENT, 0}
};

ScriptCommand::ScriptCommand(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return;
    }

    if (tokens[0][0] == ';') {
        cmd = COMMENT;
        args.push_back(tokens[0]);
        return;
    }

    if (NUM_OF_ARGS.find(tokens[0]) == NUM_OF_ARGS.end()) {
        return;
    }

    cmd = tokens[0];

    if (tokens.size() == 1) {
        return;
    }

    for (int i = 1; i < tokens.size(); i++) {
        this->args.push_back(tokens[i]);
    }
}

bool ScriptCommand::isValid() const {
    if (NUM_OF_ARGS.find(this->cmd) == NUM_OF_ARGS.end()) { return false; }

    if (this->cmd == COMMENT) {
        return true;
    } else {
        return this->args.size() >= NUM_OF_ARGS.at(this->cmd);
    }
}

std::string ScriptCommand::toString() const {
    std::ostringstream oss;

    if (this->cmd == COMMENT) {
        for (const std::string& s : this->args) {
            oss << s;
        }
    } else {
        oss << this->cmd << "\t";
        for (auto arg : this->args) {
            oss << arg << "\t";
        }
    }

    return oss.str();
}
