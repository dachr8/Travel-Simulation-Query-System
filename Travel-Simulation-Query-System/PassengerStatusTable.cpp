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
	int tmp;

	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			string* id = new string;
			PassengerRequirements* passengerRequirements = new PassengerRequirements;
			infile >> *id >> tmp;
			passengerRequirements->strategy = (strategy)tmp;

			if (passengerRequirements->strategy == limitedTime)
				infile >> passengerRequirements->timeLimit;

			infile >> tmp;
			for (int j = 0; j < tmp; ++j) {
				string tmp;
				infile >> tmp;
				passengerRequirements->cities.push_back(tmp);
			}
			addPassenger(*id, *passengerRequirements);
		}

	infile.close();
	return true;
}

bool PassengerStatusTable::delPassenger(const string& id) {
	return passengerTable.erase(id);
}

TravelSchedule* PassengerStatusTable::getTravelSchedule(const unordered_multimap<string, ArcCity>& map, const string& id) {
	PassengerRequirements requires = passengerTable.find(id)->second;
	TravelSchedule* schedule = new TravelSchedule;
	schedule->departure = requires.cities.front();

	switch (requires.strategy) {
	case minCost:
		break;
	case minTime:
		schedule->cities;
		schedule->planCost = 999;
		schedule->planTime = 666;
		break;
	case limitedTime:
		break;
	default:
		delete schedule;
		break;
	}

	return schedule;
}

PassengerStatus PassengerStatusTable::getPassengerStatus(const unordered_multimap<string, ArcCity>& map, const string& id) {
	return PassengerStatus();
}

