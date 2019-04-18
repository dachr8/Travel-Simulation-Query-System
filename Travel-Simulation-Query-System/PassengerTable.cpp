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
				string city;
				infile >> city;
				if (j == 0)
					requirements->departure = city;
				else if (j == tmp - 1)
					requirements->destination = city;
				else
					requirements->wayCities.push_back(city);
			}
			addPassenger(*id, *requirements);
		}

	infile.close();
	return true;
}

bool PassengerTable::delPassenger(const string & id) {
	return passengerRequirements.erase(id);
}

bool PassengerTable::findPassenger(const string & id) {
	return  passengerRequirements.find(id) != passengerRequirements.end();
}

const TravelSchedule& PassengerTable::getTravelSchedule(const string & id) {
	if (travelSchedule.find(id) == travelSchedule.end())
		return generateTravelSchedule(id);
	else
		return travelSchedule.find(id)->second;
}

const TravelSchedule& PassengerTable::generateTravelSchedule(const string & id) {
	PassengerRequirements requires = passengerRequirements.find(id)->second;
	TravelSchedule* schedule = new TravelSchedule;
	schedule->departure = requires.departure;
	schedule->destination = requires.destination;
	schedule->planCost = 0;

	list<string> destinations = requires.wayCities;
	string currentCity = requires.departure;
	bool find = false;

	switch (requires.strategy) {
	case minCost:
		schedule->cities;
		schedule->planCost = 111;
		break;
	case minTime:
		//
		while (currentCity != requires.destination) {
			find = false;
			for (auto i = graph->getCityMap().equal_range(currentCity); !find && i.first != i.second; ++i.first)
				if (destinations.size()) {
					auto nextCity = destinations.begin();
					while (!find && nextCity != destinations.end())
						if (i.first->second.city == *nextCity) {
							schedule->cities.push_back(i.first->second);
							destinations.erase(nextCity);
							currentCity = i.first->second.city;
							schedule->planCost += i.first->second.fare;
							find = true;
						} else
							++nextCity;
				} else if (i.first->second.city == requires.destination) {
					schedule->cities.push_back(i.first->second);
					currentCity = requires.destination;
					find = true;
				}
		}
		//
		break;
	case limitedTime:
		schedule->cities;
		schedule->planCost = 111;
		break;
	default:
		delete schedule;
		break;
	}

	if (schedule->cities.size())
		schedule->planTime = schedule->cities.back().time[1];
	else
		schedule->planTime = 999;
	if (find) {
		schedule->status.currentCity = schedule->departure;
		schedule->status.currentStatus = waiting;
		schedule->status.currentWay = schedule->cities.front();
		travelSchedule.insert({ id,*schedule });
	}
	return *schedule;
}

bool PassengerTable::printTravelSchedule(const string & id) {
	TravelSchedule schedule = getTravelSchedule(id);
	cout << id << '\t' << schedule.status.currentCity << endl;
	for (auto iter = schedule.cities.begin(); iter != schedule.cities.end(); ++iter)
		cout << ">>" << *iter << endl;
	cout << "Plan Cost: " << schedule.planCost << "\tPlan Time: ";

	tm ltm = { 0 };
	localtime_s(&ltm, &schedule.planTime);
	ltm.tm_min = 0;
	ltm.tm_sec = 0;
	printTime(ltm); return true;
}


bool PassengerTable::updatePassengerStatusTable() {
	for (auto iter = travelSchedule.begin(); iter != travelSchedule.end(); ++iter) {
		while (iter->second.status.currentStatus != over && now >= iter->second.status.currentWay.time[1]) {
			iter->second.status.currentCity = iter->second.status.currentWay.city;
			iter->second.cities.pop_front();
			if (iter->second.status.currentCity == iter->second.destination) {
				iter->second.status.currentStatus = over;
				iter->second.planCost = 0;
			} else {
				iter->second.planCost -= iter->second.status.currentWay.fare;
				iter->second.status.currentWay = iter->second.cities.front();
				if (now <= iter->second.status.currentWay.time[1])
					iter->second.status.currentStatus = waiting;
				else
					iter->second.status.currentStatus = onTheWay;
			}
			cout << iter->first << "\t到达" << iter->second.status << endl;
		}

		if (iter->second.status.currentCity == iter->second.departure && now <= iter->second.status.currentWay.time[0])
			cout << iter->first << "\t需要到达" << iter->second.status << endl;

		cout << endl;
	}
	return true;
}

bool PassengerTable::printPassengerStatusTable() {
	for (auto iter = travelSchedule.begin(); iter != travelSchedule.end(); ++iter)
		cout << iter->first << '\t' << iter->second.status << endl;
	return true;
}

void PassengerTable::printTime(const tm & ltm) {
	cout << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday << " " << ltm.tm_hour << ":00:00" << endl;
}

ostream& operator<<(ostream & os, PassengerStatus & passengerStatus) {
	os << passengerStatus.currentCity << ",";
	switch (passengerStatus.currentStatus) {
	case waiting:
		os << "等待去往";
		break;
	case onTheWay:
		os << "正在去往";
		break;
	case resting:
		os << "换乘休息中，即将去往";
		break;
	case over:
		os << "旅程已结束";
		break;
	default:
		os << "default";
		break;
	}
	if (passengerStatus.currentStatus != over)
		os << passengerStatus.currentWay;
	return os;
}
