#include <iostream>

#include "Gis.h"

void printUsage(const char *programName) {
    std::cout << "Usage: ./" << programName << " databaseFile commandFile logFile" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
    }

    std::string databaseFile{ argv[1] };
    std::string commandFile{ argv[2] };
    std::string logFile{ argv[3] };

    Gis gis(databaseFile, commandFile, logFile);

    return 0;
}