#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

class Logger {
public:
	Logger();
	Logger(const string& file);
	~Logger();

	void out(const string& content);
	void time(time_t t);
private:
	ofstream logger;
};
