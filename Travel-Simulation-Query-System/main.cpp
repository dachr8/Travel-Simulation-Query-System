#include "Graph.h"
#include "PassengerTable.h"
#include <windows.h>
#include <thread>

using namespace std;

bool timer_thread = true;
time_t now;
PassengerTable* passengers;
Graph* graph;

void timer() {
	now = time(0);
	tm ltm = { 0 };
	localtime_s(&ltm, &now);
	ltm.tm_min = 0;
	ltm.tm_sec = 0;

	while (true) {
		if (timer_thread) {
			passengers->printTime(ltm);
			passengers->updatePassengerStatusTable();
			Sleep(1000); //Sleep(10000);
			++ltm.tm_hour;
			now = mktime(&ltm);
		} else
			Sleep(1000);
	}
}

int main() {
	passengers = new PassengerTable("passengers.txt", 10);
	graph = new Graph("map.txt", 10);


	thread t(timer);
	t.detach();

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