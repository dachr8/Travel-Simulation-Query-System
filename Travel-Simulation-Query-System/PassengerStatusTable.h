#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include "Graph.h"

using namespace std;

enum status {
	waiting, onTheWay£¬resting
};

enum strategy {
	minCost, minTime, limitedTime
};

class PassengerRequirements {
public:
	list<string> cities;
	strategy strategy;
	int timeLimit;
};

class TravelSchedule {
public:
	list<string> cities;
	int planTime;
	float planCost;
};

class PassengerStatus {
public:
	status currentStatus;
	string currentCity;
};

class PassengerStatusTable {
public:
	PassengerStatusTable(const string& file, const int& num);
	~PassengerStatusTable();
	//...
	bool addPassenger(const string& id, const PassengerRequirements& newPassenger);
	bool addPassengerList(const string& file, const int& num);
	bool delPassenger(const string& id);
	TravelSchedule getTravelSchedule(Graph map, string id);
	PassengerStatus getPassengerStatus(Graph map, string id);
private:
	unordered_map<string, PassengerRequirements> passengerTable;
};
