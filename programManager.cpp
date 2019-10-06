/*
 * programManager.cpp
 *
 *      Author: Serena Cheng
 */

#include "programManager.h"
#include "operation.h"
#include <fstream>
#include <iostream>

using namespace std;


programManager::programManager() {}
programManager::~programManager() {}

vector<program> programManager::getProcesses() {
	processes.shrink_to_fit();
	return processes;
}

void programManager::createProgram(int programNumber, int numberToMake) {

	pair<string, string> data = chooseFile(programNumber);

	for(int i = 0; i < numberToMake; i++) {

		program process(data.first, data.second);
		dp.updateState(New, process.getPcb());

		processes.push_back(process);
		openProgram(process);
	}

}

pair<string, string> programManager::chooseFile(int number) {

	string filePath = "template files/program_file";

	switch(number) {

	case 1:
		return make_pair("Text Processor", filePath + "1.txt");
	case 2:
		return make_pair("Web Browser", filePath + "2.txt");
	case 3:
		return make_pair("Photo Editor", filePath + "3.txt");
	case 4:
		return make_pair("Music Player", filePath + "4.txt");
	default:
		break;

	}

	return make_pair("", "");

}

void programManager::openProgram(program process) {

	ifstream inFile;

	inFile.open(process.getFilePath());

	if (!inFile) {

		cout << "Unable to open file.";
		exit(1);

	} else {
		readFile(&inFile, process);
	}

	inFile.close();

}

ifstream * programManager::goToLine(ifstream *inFile, int lineNumber, program process) {

	inFile->seekg(ios::beg);

	for (int i = 0; i < lineNumber - 1; i++) {
		inFile->ignore(1000,'\n');
	}

	return inFile;

}

void programManager::readFile(ifstream *inFile, program process) {

	int linePC = process.getPcb()->getPc();

	goToLine(inFile, linePC, process);

	string line;
	while (getline(*inFile, line)) {

		process.getPcb()->setPc(linePC++);

		if (line.find("CALCULATE") != -1 || line.find("I/O") != -1 || line.find("YIELD") != -1 ||
				line.find("OUT") != -1) {
			cout << line << endl;
		} else if (line.find("EXE") != -1) {

		}

	}

}
