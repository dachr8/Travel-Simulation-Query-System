#include "logger.h"

Logger::Logger() {
	logger.open("log.txt");
}

Logger::Logger(const string& file) {
	logger.open(file);
}

Logger::~Logger() {
	logger.close();
}

void Logger::out(const string& content) {
	cout << content;
	logger << content;
}

void Logger::time(time_t t) {
	char tmp[64];
	tm ltm;
	localtime_s(&ltm, &t);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &ltm);

	cout << tmp << endl;
	logger << tmp << endl;
}
