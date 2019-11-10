/*
 * processManager.cpp
 *
 * Author: Serena Cheng
 */

#include "processManager.h"
#include <fstream>
#include <iostream>
#include <random>

using namespace std;


processManager::processManager() {}
processManager::~processManager() {}

vector<process> processManager::getProcesses() {
	processes.shrink_to_fit();
	return processes;
}

void processManager::createProcess(int processNumber, int numberToMake) {

	pair<string, string> data = chooseFile(processNumber);

	for(int i = 0; i < numberToMake; i++) {

		process process(data.first, data.second);
		dp.updateState(New, process.getPcb());

		processes.push_back(process);

	}

}

pair<string, string> processManager::chooseFile(int number) {

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

void processManager::openProcess(process *process) {

	ifstream inFile;

	inFile.open(process->getFilePath());

	if (!inFile) {

		cout << "Unable to open file.";
		exit(1);

	} else {
		readFile(&inFile, *process);
	}

	inFile.close();

}

ifstream * processManager::goToLine(ifstream *inFile, int lineNumber, process &process) {

	inFile->seekg(ios::beg);

	for (int i = 0; i < lineNumber - 1; i++) {
		inFile->ignore(1000,'\n');
	}

	return inFile;

}

void processManager::readFile(ifstream *inFile, process &process) {

	int linePC = process.getPcb()->getPc();

	goToLine(inFile, linePC, process);

	string line;
	while (getline(*inFile, line)) {

		process.getPcb()->setPc(++linePC);

		if (process.getPcb()->getMemory() == 0) {

			if (line.find("Memory") != -1) {
				string size = line.substr(line.find(" ")+1, line.length());
				int memSize = atoi(size.c_str());
				process.getPcb()->setMemory(memSize);
			}

		}

		if (line.find("CALCULATE") != -1) {
			dp.updateState(Run, process.getPcb());
			op.calculate(generateRandomNumber());
		} else if (line.find("I/O") != -1) {
			op.wait(generateRandomNumber());
		} else if (line.find("YIELD") != -1) {
			op.yield();
		} else if (line.find("OUT") != -1) {
			op.out(process);
		} else if (line.find("EXE") != -1) {
			op.exit();
		}

	}

}

int processManager::generateRandomNumber() {

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> range(50, 100);
	int x = range(gen);
//	cout << "rand " << x << endl;
	return x;

}