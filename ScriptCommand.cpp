#include "ScriptCommand.h"

#include <sstream>

const std::unordered_map<std::string, int> ScriptCommand::NUM_OF_ARGS = {
    {"world", 4},
    {"import", 1},
    {"debug", 1},
    {"quit", 0},
    {"what_is_at", 2},
    {"what_is", 2},
    {"what_is_in", 4}
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
        for (int i = 0; i < NUM_OF_ARGS.at(cmd); i++) {
            args.push_back(tokens[i + 1]);
        }
    }
}

bool ScriptCommand::isValid() {
    return NUM_OF_ARGS.find(this->cmd) != NUM_OF_ARGS.end();
}

std::string ScriptCommand::toString() const {
    std::ostringstream oss;

    oss << cmd << " ";

    for (auto arg : args) {
        oss << arg << " ";
    }

    return oss.str();
}
