#include <iostream>
#include "Graph.h"
#include "PassengerStatusTable.h"

using namespace std;



int main() {
	PassengerStatusTable passengerStatusTable("passengers.txt", 1);
	Graph map("map.txt", 10);


	time_t now = time(0);
	tm ltm = { 0 };
	localtime_s(&ltm, &now);

	cout << 1900 + ltm.tm_year << "Äê " << 1 + ltm.tm_mon << "ÔÂ ";
	cout << ltm.tm_mday << "ÈÕ " << ltm.tm_hour << ":" << ltm.tm_min << ":" << ltm.tm_sec << endl;


	// ...


	return 0;
}