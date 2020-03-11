#include "FileTokenizer.h"

#include <iostream>

FileTokenizer::FileTokenizer(const std::string& filename, const std::string& delim)
    : delim(delim), filename(filename), in(filename) {}

FileTokenizer::~FileTokenizer() {
    this->in.close();
}

int FileTokenizer::getNextLineAsTokens(std::vector<std::string>& tokens) {
    if (!this->in.is_open()) {
        return 0;
    }

    size_t pos;
    std::string line;
    std::getline(this->in, line);

    tokens.clear();
    while (line.length() > 0 && line[0] != ';') {
        pos = line.find(this->delim);

        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + this->delim.length());

        if (pos == std::string::npos) {
            break;
        }
    }

    return tokens.size();
}

bool FileTokenizer::eof() {
    return this->in.eof();
}