#include "FileTokenizer.h"

#include <algorithm>
#include <iostream>

#include "utils.h"

FileTokenizer::FileTokenizer(const std::string& filename, const std::string& delim)
    : delim(delim), filename(filename), ifs(filename) {}

FileTokenizer::~FileTokenizer() {
    this->ifs.close();
}

std::vector<std::string> FileTokenizer::getNextLineAsTokens(const bool skipEmpty) {
    if (!this->ifs.is_open()) {
        return std::vector<std::string>();
    }

    std::string line;
    std::getline(this->ifs, line);

    if (line[0] == ';') {
        return utils::split(line, " ", skipEmpty);
    }

    return utils::split(line, this->delim, skipEmpty);
}

bool FileTokenizer::eof() {
    return this->ifs.eof();
}