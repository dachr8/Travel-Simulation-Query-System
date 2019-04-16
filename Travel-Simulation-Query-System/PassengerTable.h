#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include <ctime>
#include "Graph.h"

using namespace std;

extern bool timer_thread;
extern time_t now;


enum status {
	waiting, onTheWay, resting, over
};

enum strategy {
	minCost, minTime, limitedTime
};

class PassengerRequirements {
public:
	list<string> cities;
	strategy strategy;
	time_t timeLimit;
};

class TravelSchedule {
public:
	string departure;
	list<ArcCity> cities;
	time_t planTime;
	float planCost;
};

class PassengerStatus {
public:
	status currentStatus;
	string currentCity;
};

ostream& operator<<(ostream& os, PassengerStatus& passengerStatus);

class PassengerTable {
public:
	PassengerTable();
	PassengerTable(const string& file, const int& num);
	~PassengerTable();
	//...
	bool addPassenger(const string& id, const PassengerRequirements& newPassenger);
	bool addPassengerList(const string& file, const int& num);
	bool delPassenger(const string& id);
	const TravelSchedule& getTravelSchedule(const unordered_multimap<string, ArcCity>& map, const string& id);
	const PassengerStatus& getPassengerStatus(const string& id);
	bool printPassengerStatusTable();
private:
	unordered_map<string, PassengerRequirements> passengerRequirements;
	unordered_map<string, TravelSchedule> travelSchedule;
	unordered_map<string, PassengerStatus> passengerStatusTable;
};
