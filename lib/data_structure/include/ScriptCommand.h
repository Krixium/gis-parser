#include <string>
#include <unordered_map>
#include <vector>

#define CMD_WORLD "world"
#define CMD_IMPORT "import"
#define CMD_DEBUG "debug"
#define CMD_QUIT "quit"
#define CMD_WHAT_IS_AT "what_is_at"
#define CMD_WHAT_IS "what_is"
#define CMD_WHAT_IS_IN "what_is_in"
#define COMMENT ";"

class ScriptCommand {
public:
private:
    static const std::unordered_map<std::string, int> NUM_OF_ARGS;

    std::string cmd = "";
    std::vector<std::string> args;

public:
    ScriptCommand(const std::vector<std::string>& tokens);

    bool isValid() const;

    inline const std::string& getCmd() const { return this->cmd; }
    inline const std::vector<std::string>& getArgs() const { return this->args; }

    std::string toString() const;

    inline friend std::ostream& operator<<(std::ostream& os, const ScriptCommand& cmd) {
        os << cmd.toString();
        return os;
    }
};