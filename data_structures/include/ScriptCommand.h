#include <string>
#include <vector>
#include <unordered_map>

class ScriptCommand {
public:
    static constexpr char* CMD_WORLD = "world";
    static constexpr char* CMD_IMPORT = "import";
    static constexpr char* CMD_DEBUG = "debug";
    static constexpr char* CMD_QUIT = "quit";
    static constexpr char* CMD_WHAT_IS_AT = "what_is_at";
    static constexpr char* CMD_WHAT_IS = "what_is";
    static constexpr char* CMD_WHAT_IS_IN = "what_is_in";
    static constexpr char* COMMENT = ";";

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