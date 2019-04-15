#include <iostream>
#include "Graph.h"
#include "PassengerStatusTable.h"

using namespace std;



int main() {
	PassengerStatusTable passengers("passengers.txt", 1);
	Graph map("map.txt", 10);

	passengers.getTravelSchedule(map.getGraph(), "zhang");

	// ...


	return 0;
}