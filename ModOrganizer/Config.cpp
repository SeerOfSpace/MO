#include "Config.h"
#include <iostream>
#include <fstream>
using namespace std;

int Config::getLength(const string *array)
{
	int i = 0;
	while (!array[i].empty()) {
		cout << array[i] << endl;
		i++;
	}
	return i;
}

vector<string> Config::readConfig(vector<string> values) {
	vector<string> values2(values.size(), "null");
	//fill(begin(values2), end(values2), "null");
	string line;
	ifstream file("test.txt");
	while (getline(file, line)) {
		for (int i = 0; i < values.size(); i++) {
			if (values[i] == line.substr(0, line.find("="))) {
				values2[i] = line.substr(line.find("=") + 1, line.length());
			}
		}
	}
	for (string s : values2) {
		cout << s << endl;
	}
	return values2;
}