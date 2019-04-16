#include "Graph.h"
#include "PassengerTable.h"
#include <windows.h>
#include <thread>

using namespace std;

bool timer_thread = true;
time_t now;

void timer() {
	now = time(0);
	tm ltm = { 0 };
	localtime_s(&ltm, &now);
	ltm.tm_min = 0;
	ltm.tm_sec = 0;


	while (true) {
		if (timer_thread) {
			cout << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday << " " << ltm.tm_hour << ":00:00" << endl;
			Sleep(1000);
			//Sleep(10000);
			++ltm.tm_hour;
			now = mktime(&ltm);
		} else
			Sleep(1000);
	}
}

int main() {
	PassengerTable passengers("passengers.txt", 1);
	Graph map("map.txt", 10);


	thread t(timer);
	t.detach();

	bool run_flag = true;
	while (run_flag) {
		string cmd;
		cin >> cmd;
		if (cmd == "q") {
			run_flag = false;
		} else if (cmd == "zhang") {
			if (timer_thread) {
				timer_thread = false;
				TravelSchedule schedule = passengers.getTravelSchedule(map.getGraph(), "zhang");
				cout << "zhang " << schedule.departure << endl;
				for (auto iter = schedule.cities.begin(); iter != schedule.cities.end(); ++iter)
					cout << ">>" << *iter << endl;
				cout << "Plan Cost: " << schedule.planCost << "\tPlan Time: " << schedule.planTime << endl << endl;
				timer_thread = true;
			}
		} else if (cmd == "check") {
			if (timer_thread) {
				timer_thread = false;
				passengers.printPassengerStatusTable();
				cout << endl;
				timer_thread = true;
			}
		}
	}



	// ...


	return 0;
}