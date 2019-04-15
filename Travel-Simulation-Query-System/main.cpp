#include "Graph.h"
#include "PassengerStatusTable.h"
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
	PassengerStatusTable passengers("passengers.txt", 1);
	Graph map("map.txt", 10);


	thread t(timer);
	t.detach();

	while (true) {
		char switch_on;
		cin >> switch_on;
		switch (switch_on) {
		case 's':
			if (timer_thread) {
				timer_thread = false;
				TravelSchedule* schedule = passengers.getTravelSchedule(map.getGraph(), "zhang");
				cout << "zhang " << schedule->departure << endl;
				for (auto iter = schedule->cities.begin(); iter != schedule->cities.end(); ++iter)
					cout << ">>" << *iter << endl;
				cout << "Plan Cost: " << schedule->planCost << "\tPlan Time: " << schedule->planTime << endl;
				delete schedule;
				timer_thread = true;
			}
			break;
		default:
			break;
		}
	}



	// ...


	return 0;
}