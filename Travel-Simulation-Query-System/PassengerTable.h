#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include <ctime>
#include "TimeTable.h"

using namespace std;

extern bool timer_thread;
extern time_t now;
extern TimeTable* timeTable;


enum status {
	waiting, onTheWay, resting, over
};

enum strategy {
	minCost, minTime, limitedTime
};

class PassengerRequirements {
public:
	string departure;
	string destination;
	list<string> wayCities;
	strategy strategy;
	time_t timeLimit;
};

class PassengerStatus {
public:
	status currentStatus;
	string currentCity;
	ArcCity currentWay;
};

class TravelSchedule {
public:
	string departure;
	string destination;
	list<ArcCity> cities;
	time_t planTime = 0;
	float planCost = 0;
	PassengerStatus status;
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
	bool findPassenger(const string& id);
	const TravelSchedule& getTravelSchedule(const string& id);
	const TravelSchedule& generateTravelSchedule(const string& id);
	bool updatePassengerStatusTable();
	bool printTravelSchedule(const string& id);
	bool printPassengerStatusTable();
	void printTime(const tm& ltm);

private:
	unordered_map<string, PassengerRequirements> passengerRequirements;
	unordered_map<string, TravelSchedule> travelSchedule;
};
