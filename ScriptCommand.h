#include <string>
#include <vector>
#include <unordered_map>

class ScriptCommand {
private:
    static const std::unordered_map<std::string, int> NUM_OF_ARGS;

    std::string cmd;
    std::vector<std::string> args;

public:
    ScriptCommand(const std::vector<std::string>& tokens);

    bool isValid();

    std::string toString() const;
    
    inline friend std::ostream& operator<<(std::ostream& os, const ScriptCommand& cmd) {
        os << cmd.toString();
        return os;
    }
};