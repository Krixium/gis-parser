#include <iostream>

#include "CircularQueue.h"
#include "HashMap.h"
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
    HashMap<int> hm(10);

    for (int i = 0; i < 10; i++) {
        hm.insert(i);
    }

    for (int i = 0; i < 15; i++) {
        std::cout << "i: " << i  << " " << hm.search(i) << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // testCommandParsing();
    // testEntryParsing();
    // testCircularQueue();
    // testHashMap();

    system("pause");
    return 0;
}