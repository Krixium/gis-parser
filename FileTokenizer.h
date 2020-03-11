#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

class FileTokenizer {
private:
    std::string delim;
    std::string filename;
    std::ifstream in;

public:
    FileTokenizer(const std::string& filename, const std::string& delim);
    ~FileTokenizer();

    FileTokenizer(const FileTokenizer& other) = delete;
    FileTokenizer(FileTokenizer&& other) = delete;
    FileTokenizer& operator=(const FileTokenizer& other) = delete;
    FileTokenizer& operator=(FileTokenizer&& other) = delete;

    int getNextLineAsTokens(std::vector<std::string>& tokens);
    bool eof();
};
