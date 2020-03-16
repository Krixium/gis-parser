#include "FileTokenizer.h"

#include <algorithm>
#include <iostream>

#include "utils.h"

FileTokenizer::FileTokenizer(const std::string& filename, const std::string& delim)
    : delim(delim), filename(filename), in(filename) {}

FileTokenizer::~FileTokenizer() {
    this->in.close();
}

std::vector<std::string> FileTokenizer::getNextLineAsTokens(const bool skipEmpty) {
    if (!this->in.is_open()) {
        return std::vector<std::string>();
    }

    std::string line;
    std::getline(this->in, line);

    return split(line, this->delim, skipEmpty);
}

bool FileTokenizer::eof() {
    return this->in.eof();
}