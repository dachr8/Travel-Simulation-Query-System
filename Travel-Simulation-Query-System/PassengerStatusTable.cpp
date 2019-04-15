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
			passengerRequirements->cities.resize(tmp);

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

	time_t now = time(0);
	tm ltm = { 0 };
	localtime_s(&ltm, &now);

	switch (requires.strategy) {
	case minCost:
		break;
	case minTime:


		cout << 1900 + ltm.tm_year << "Äê " << 1 + ltm.tm_mon << "ÔÂ ";
		cout << ltm.tm_mday << "ÈÕ " << ltm.tm_hour << ":" << ltm.tm_min << ":" << ltm.tm_sec << endl;

		break;
	case limitedTime:
		break;
	default:
		break;
	}

	TravelSchedule* schedule = new TravelSchedule;
	schedule->cities;
	schedule->planCost;
	schedule->planTime;
	return schedule;
}

PassengerStatus PassengerStatusTable::getPassengerStatus(const unordered_multimap<string, ArcCity>& map, const string& id) {
	return PassengerStatus();
}

