#include <iostream>
#include "Graph.h"
#include "PassengerStatusTable.h"

using namespace std;



int main() {
	PassengerStatusTable passengerStatusTable("passengers.txt", 10);
	Graph map("map.txt", 20);


	// ...


	cout << "S" << endl;
	return 0;
}