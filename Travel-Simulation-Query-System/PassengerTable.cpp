#include "PassengerTable.h"
#include "Permutations.h"

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
				infile >> tm_.tm_mday >> tm_.tm_hour;
				tm_.tm_min = 0;
				tm_.tm_sec = 0;
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
			requirements->timeStart = now;
			addPassenger(*id, *requirements);
		}

	infile.close();
	return true;
}

bool PassengerTable::delPassenger(const string& id) {
	return  passengerRequirements.erase(id) && travelSchedule.erase(id);
}

bool PassengerTable::findPassenger(const string& id) {
	return  passengerRequirements.find(id) != passengerRequirements.end();
}

TravelSchedule PassengerTable::getTravelSchedule(const string& id) {
	if (travelSchedule.find(id) == travelSchedule.end())
		return generateTravelSchedule(id);
	else
		return travelSchedule.find(id)->second;
}

TravelSchedule PassengerTable::reSchedule(const string& id, const PassengerRequirements& newRequire) {
	delPassenger(id);
	addPassenger(id, newRequire);
	return getTravelSchedule(id);
}

TravelSchedule PassengerTable::generateTravelSchedule(const string& id) {
	PassengerRequirements requires = passengerRequirements.find(id)->second;
	City c;
	TravelSchedule* schedule = c.Permutations(requires);

	if (schedule->status.currentStatus == error) {
		logger->out(id + "\t路径规划失败\n");
	} else {
		schedule->status.currentCity = schedule->departure;
		schedule->status.currentStatus = waiting;
		schedule->status.currentWay = schedule->cities.front();
		travelSchedule.insert({ id,*schedule });
		logger->out(id + "\t路径规划成功\n");
	}
	return *schedule;
}

bool PassengerTable::printTravelSchedule(const string& id) {
	TravelSchedule schedule = getTravelSchedule(id);

	if (schedule.status.currentStatus != error) {
		logger->out(id + '\t' + schedule.status.currentCity + "\n");
		for (auto iter = schedule.cities.begin(); iter != schedule.cities.end(); ++iter)
			logger->out(">>" + iter->toString() + "\n");
		logger->out("Plan Cost: " + to_string(schedule.planCost) + "\tPlan Time: ");
		logger->time(schedule.planTime);
	}
	logger->out("\n");
	return true;
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
			logger->out(iter->first + "\t到达" + StatustoString(iter->second.status) + "\n");
		}

		if (iter->second.status.currentCity == iter->second.departure && now <= iter->second.status.currentWay.time[0]) {
			logger->out(iter->first + "\t需要到达" + StatustoString(iter->second.status) + "\n");
		}
	}
	return true;
}

bool PassengerTable::printPassengerStatusTable() {
	for (auto iter = travelSchedule.begin(); iter != travelSchedule.end(); ++iter)
		logger->out(iter->first + '\t' + StatustoString(iter->second.status) + "\n");
	return true;
}

string PassengerTable::StatustoString(PassengerStatus& status) {
	string str = status.currentCity + ",";
	switch (status.currentStatus) {
	case waiting:
		str += "等待去往";
		break;
	case onTheWay:
		str += "正在去往";
		break;
	case resting:
		str += "换乘休息中，即将去往";
		break;
	case over:
		str += "旅程已结束";
		break;
	default:
		str += "error";
		break;
	}
	if (status.currentStatus != over)
		str += status.currentWay.toString();
	return str;
}
