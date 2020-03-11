#include <iostream>

#include "FileTokenizer.h"
#include "GeoFeature.h"
#include "ScriptCommand.h"

void testCommandParsing() {
    FileTokenizer cmdFile("./data_files/DemoScript01.txt", "\t");

    while (!cmdFile.eof()) {
        std::vector<std::string> data;
        cmdFile.getNextLineAsTokens(data);

        std::cout << ScriptCommand(data).toString() << std::endl;
    }
}

void testEntryParsing() {
    FileTokenizer entryFile("./data_files/NM_All.txt", "|");

    {
        std::vector<std::string> tmp;
        entryFile.getNextLineAsTokens(tmp);
    }

    while (!entryFile.eof()) {
        std::vector<std::string> data;
        entryFile.getNextLineAsTokens(data);

        std::cout << GeoFeature(data).toString() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    testCommandParsing();
    testEntryParsing();

    system("pause");
    return 0;
}