#include "PassengerStatusTable.h"

PassengerStatusTable::PassengerStatusTable() {
}

PassengerStatusTable::PassengerStatusTable(const string& file, const int& num) {
	addPassengerList(file, num);
}
PassengerStatusTable::~PassengerStatusTable() {
}

bool PassengerStatusTable::addPassenger(const string& id, const PassengerRequirements& newPassenger) {
	return passengerTable.insert({ id,newPassenger }).second;
}

bool PassengerStatusTable::addPassengerList(const string& file, const int& num) {
	string id;
	PassengerRequirements passengerRequirements;
	int tmp;

	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			infile >> id >> tmp;
			passengerRequirements.strategy = (strategy)tmp;

			if (passengerRequirements.strategy == limitedTime)
				infile >> passengerRequirements.timeLimit;

			infile >> tmp;
			passengerRequirements.cities.resize(tmp);

			for (int j = 0; j < tmp; ++j) {
				string tmp;
				infile >> tmp;
				passengerRequirements.cities.push_back(tmp);
			}
			addPassenger(id, passengerRequirements);
		}

	infile.close();
	return true;
}

bool PassengerStatusTable::delPassenger(const string& id) {
	return passengerTable.erase(id);
}

TravelSchedule PassengerStatusTable::getTravelSchedule(const Graph& map, const string& id) {
	return TravelSchedule();
}

PassengerStatus PassengerStatusTable::getPassengerStatus(const Graph& map, const string& id) {
	return PassengerStatus();
}

