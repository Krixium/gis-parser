#include <iostream>

#include "utils.h"

#include "CircularQueue.h"
#include "Database.h"
#include "HashMap.h"
#include "FileTokenizer.h"
#include "GeoFeature.h"
#include "ScriptCommand.h"

void testCommandParsing(const std::string& filename = "./data_files/DemoScript01.txt") {
    FileTokenizer cmdFile(filename, "\t");

    while (!cmdFile.eof()) {
        std::cout << ScriptCommand(cmdFile.getNextLineAsTokens()) << std::endl;
    }
}

void testDatabaseGenerating(const std::string& entryFileName, const std::string& databaseFileName) {
    Database::Get(databaseFileName).setBounds(0, 0, 300);
    FileTokenizer entryFile(entryFileName, "|");
    std::vector<GeoFeature> features;
    double time;

    {
        entryFile.getNextLineAsTokens();
    }

    while (!entryFile.eof()) {
        Database::Get().storeToFile(GeoFeature(entryFile.getNextLineAsTokens()));
    }

    const GeoFeature& target = Database::Get().searchByName("889215", "Frank Creek");

    {
        time = utils::timer([&]() {
            //features = Database::Get().searchByCoordinate(target.getPrimCoordDec());
            features = Database::Get().searchByCoordinate(target.getPrimCoordDec(), 0.1);
        });
    }

    for (const GeoFeature& feature : features) {
        std::cout << feature << std::endl << std::endl;
    }

    std::cout << "Search took: " << time << "s" << std::endl;
}

void testCircularQueue() {
    CircularQueue<int> queue(5);

    for (int i = 10; i < 70; i += 10) {
        std::cout << queue.enque(i) << std::endl;
    }

    for (int i = 0; i < 2; i++) {
        std::cout << queue.deque() << std::endl;
    }

    for (int i = 0; i < 4; i++) {
        std::cout << queue.enque(i) << std::endl;
    }
}

void testHashMap() {
    std::srand(75);
    HashMap<int, int> hm(10);

    for (int i = 0; i < 10; i++) {
        int value = std::rand();
        hm.insert(i, value);
        std::cout << "i: " << i << " " << value << std::endl;
    }

    for (int i = 0; i < 15; i++) {
        try {
            std::cout << "i: " << i << " " << hm.get(i) << std::endl;
        } catch (const std::exception& e) {
            std::cout << "i: " << i << " " << e.what() << std::endl;
        }
    }
}

void printUsage(const char *programName) {
    std::cout << "Usage: ./" << programName << " databaseFile commandFile logFile" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
    }

    std::string databaseFile{ argv[1] };
    std::string commandFile{ argv[2] };
    std::string logFIle{ argv[3] };

    // testCommandParsing(commandFile);
    testDatabaseGenerating("./data_files/NM_All.txt", databaseFile);
    // testCircularQueue();
    // testHashMap();

    system("pause");

    return 0;
}