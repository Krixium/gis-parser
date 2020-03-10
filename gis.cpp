#include <iostream>

#include "FileTokenizer.h"
#include "GeoFeature.h"

int main(int argc, char *argv[]) {
	FileTokenizer entryFile("./data_files/NM_All.txt", "|");

	{
		std::vector<std::string> tmp;
		entryFile.getNextLineAsTokens(tmp);
	}

	while (!entryFile.eof()) {
		std::vector<std::string> data;
		entryFile.getNextLineAsTokens(data);

		GeoFeature gf(data);

		std::cout << gf.toString() << std::endl;
	}

	system("pause");
	return 0;
}