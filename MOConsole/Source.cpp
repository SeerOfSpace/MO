#include "Logic.h"
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
namespace fs = std::experimental::filesystem;

void printColor(string s, vector<unsigned short> attributes) {
	if (attributes.size() == 0) {
		return;
	}
	unsigned short wAttributes = attributes[0];
	for (int i = 1; i < attributes.size(); i++) {
		wAttributes = wAttributes | attributes[i];
	}
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo = {};
	GetConsoleScreenBufferInfo(hStdOut, &consoleScreenBufferInfo);
	unsigned short temp = consoleScreenBufferInfo.wAttributes;
	SetConsoleTextAttribute(hStdOut, wAttributes);
	cout << s << endl;
	SetConsoleTextAttribute(hStdOut, temp);
}

class LogicI : public MOLogic {

public:

	bool enableMinorOutput = true;

	LogicI(MOLogicStruct moStruct)
		:MOLogic(moStruct) {

	}

	virtual void minorOutput(string s) {
		if (enableMinorOutput) {
			printColor(s, { FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_INTENSITY });
		}
	}

	virtual void majorOutput(string s) {
		printColor(s, { FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_INTENSITY });
	}

	virtual void errorOutput(string s) {
		printColor(s, { FOREGROUND_RED, FOREGROUND_INTENSITY });
	}

	virtual bool confirmation(string s) {
		string a;
		cout << s;
		cout << " Y/N: ";
		cin >> a;
		if (a == "Y" || a == "y") {
			return true;
		}
		else if (a == "N" || a == "n") {
			return false;
		}
		else {
			bool b = confirmation(s);
			return b;
		}
	}

};

inline std::wstring convert(const std::string& s)
{
	// deal with trivial case of empty string
	if (s.empty()) {
		return std::wstring();
	}

	// determine required length of new string
	size_t reqLength = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.length(), 0, 0);

	// construct new string of required length
	std::wstring ws(reqLength, L'\0');

	// convert old string to new string
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.length(), &ws[0], (int)ws.length());

	// return new string ( compiler should optimize this away )
	return ws;
}

string removeSpaces(string s) {
	int index = s.find("=");
	if (s.at(index - 1) == ' ') {
		s = s.substr(0, index - 1) + s.substr(index, s.length());
		index--;
	}
	if (s.at(index + 1) == ' ') {
		s = s.substr(0, index + 1) + s.substr(index + 2, s.length());
	}
	return s;
}

vector<string> readConfig(vector<string> values, string fileName) {
	vector<string> values2(values.size(), "");
	string line;
	ifstream file(fileName);
	if (!file.is_open()) {
		return values2;
	}
	while (getline(file, line)) {
		string temp = removeSpaces(line);
		int index = temp.find("=");
		for (int i = 0; i < values.size(); i++) {
			if (values[i] == temp.substr(0, index)) {
				values2[i] = temp.substr(index + 1, line.length());
			}
		}
	}
	return values2;
}

vector<string> readMods(string fileName) {
	vector<string> v;
	string line;
	ifstream file(fileName);
	if (!file.is_open()) {
		return v;
	}
	while (getline(file, line)) {
		if (line.substr(0, 2) != "//") {
			v.push_back(line);
		}
	}
	return v;
}

LogicI *logicI;

BOOL ctrl_handler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		logicI->stop();
		Sleep(100000);
		return TRUE;
	}
	return FALSE;
}

int main() {
	vector<string> v1 = readConfig({"modPath", "gamePath", "exePath", "processName", "appID", "minorOutput", "modList"}, "Config.ini");
	vector<string> v2 = readMods(v1[6]);
	MOLogicStruct moStruct;
	moStruct.modPath = v1[0];
	moStruct.gamePath = v1[1];
	moStruct.exePath = v1[2];
	moStruct.processName = convert(v1[3]);
	if (v1[4] != "") {
		moStruct.appID = (unsigned int)stoul(v1[4]);
	}
	for (string s : v2) {
		moStruct.mods.push_back(s);
	}
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);
	logicI = new LogicI(moStruct);
	if (v1[5] == "false") {
		logicI->enableMinorOutput = false;
	}
	logicI->majorOutput("Starting MO");
	Sleep(1000);
	logicI->start();
	Sleep(2000);
	delete logicI;

	//system("pause");
	return 0;
}