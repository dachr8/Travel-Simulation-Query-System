#include "PassengerTable.h"

PassengerTable::PassengerTable() {
}

PassengerTable::PassengerTable(const string& file, const int& num) {
	addPassengerList(file, num);
}
PassengerTable::~PassengerTable() {
}

bool PassengerTable::addPassenger(const string& id, const PassengerRequirements& newPassenger) {
	return passengerRequirements.insert({ id,newPassenger }).second;
}

bool PassengerTable::addPassengerList(const string& file, const int& num) {
	int tmp;

	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			string* id = new string;
			PassengerRequirements* requirements = new PassengerRequirements;
			infile >> *id >> tmp;
			requirements->strategy = (strategy)tmp;

			if (requirements->strategy == limitedTime) {
				tm tm_ = { 0 };
				int tmp;
				infile >> tmp;
				tm_.tm_year = tmp - 1900;
				infile >> tmp;
				tm_.tm_mon = tmp - 1;
				infile >> tm_.tm_mday >> tm_.tm_hour >> tm_.tm_min >> tm_.tm_sec;
				requirements->timeLimit = mktime(&tm_);
			}

			infile >> tmp;
			for (int j = 0; j < tmp; ++j) {
				string tmp;
				infile >> tmp;
				requirements->cities.push_back(tmp);
			}
			addPassenger(*id, *requirements);
		}

	infile.close();
	return true;
}

bool PassengerTable::delPassenger(const string& id) {
	return passengerRequirements.erase(id);
}

bool PassengerTable::findPassenger(const string& id) {
	return  passengerRequirements.find(id) != passengerRequirements.end();
}

const TravelSchedule& PassengerTable::getTravelSchedule(const unordered_multimap<string, ArcCity>& map, const string& id) {
	if (travelSchedule.find(id) == travelSchedule.end())
		return generateTravelSchedule(map, id);
	else
		return travelSchedule.find(id)->second;
}

const TravelSchedule& PassengerTable::generateTravelSchedule(const unordered_multimap<string, ArcCity> & map, const string & id) {
	PassengerRequirements requires = passengerRequirements.find(id)->second;
	TravelSchedule* schedule = new TravelSchedule;
	schedule->departure = requires.cities.front();

	switch (requires.strategy) {
	case minCost:
		schedule->cities;
		schedule->planCost = 111;
		schedule->planTime = 222;
		break;
	case minTime:
		schedule->cities;
		schedule->planCost = 999;
		schedule->planTime = 666;
		break;
	case limitedTime:
		schedule->cities;
		schedule->planCost = 111;
		schedule->planTime = requires.timeLimit;
		break;
	default:
		delete schedule;
		break;
	}
	travelSchedule.insert({ id,*schedule });
	passengerStatusTable.insert({ id, { waiting,schedule->departure} });
	return *schedule;
}

bool PassengerTable::updatePassengerStatusTable() {
	cout << "//" << endl;
	return true;
}

bool PassengerTable::printPassengerStatusTable() {
	for (auto iter = passengerStatusTable.begin(); iter != passengerStatusTable.end(); ++iter)
		cout << iter->first << '\t' << iter->second << endl;
	return true;
}

ostream& operator<<(ostream & os, PassengerStatus & passengerStatus) {
	os << passengerStatus.currentCity << '\t';
	switch (passengerStatus.currentStatus) {
	case waiting:
		os << "等待中";
		break;
	case onTheWay:
		os << "在途中";
		break;
	case resting:
		os << "换乘休息中";
		break;
	case over:
		os << "旅程已结束";
		break;
	default:
		os << "default";
		break;
	}

	return os;
}
