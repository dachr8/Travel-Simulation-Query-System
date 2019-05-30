#include "TimeTable.h"
#include "PassengerTable.h"
#include "transmission_protocol.h"
#include "Logger.h"

#include "ws_server.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __unix__
#include <unistd.h>
#endif

#include <thread>

using namespace std;

bool timer_thread = true;
time_t now;
PassengerTable* passengers;
TimeTable* timeTable;
Logger* logger;

void timer(time_t start) {
	now = start;
	tm ltm = { 0 };
#ifdef _WIN32
	localtime_s(&ltm, &now);
#endif
#ifdef __unix__
	localtime_r(&now, &ltm);
#endif
	ltm.tm_min = 0;
	ltm.tm_sec = 0;

	while (true) {
		if (timer_thread) {
			logger->time(now);
			passengers->updatePassengerStatusTable();
			logger->out("\n");
#ifdef _WIN32
			Sleep(1000);
#endif
#ifdef __unix__
			usleep(1000000);
#endif
			++ltm.tm_hour;
			now = mktime(&ltm);
		} else {
#ifdef _WIN32
			Sleep(1000);
#endif
#ifdef __unix__
			usleep(1000000);
#endif
		}
	}
}


int main() {
	passengers = new PassengerTable;
	timeTable = new TimeTable("map.txt", 567);
	logger = new Logger("log.txt");

	thread t(timer,
		//             1558843200
		time(0)
	);
	t.detach();

	fdt::start_ws_server();

	//system("cd ../../ & cd FDTsFrontend/ & index.html");

	bool run_flag = true;
	while (run_flag) {
		string cmd;
		cin >> cmd;
		if (cmd == "#") {
			run_flag = false;
		} else if (cmd == "check") {
			timer_thread = false;
			passengers->printPassengerStatusTable();
			cout << endl;
			timer_thread = true;
		} else if (passengers->findPassenger(cmd)) {
			timer_thread = false;
			passengers->printTravelSchedule(cmd);
			cout << endl;
			timer_thread = true;
		}
	}

	return 0;
}

