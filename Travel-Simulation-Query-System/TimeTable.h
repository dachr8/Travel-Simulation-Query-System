#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>
#include <ctime>

using namespace std;

class ArcCity {
public:
	string city;
	string transportation;
	float fare;
	time_t time[2] = { 0 };
	string toString();
};

bool operator==(const ArcCity& a, const ArcCity& b);

class TimeTable {
public:
	TimeTable();
	TimeTable(const string& file, const int& num);
	~TimeTable();
	bool addArc(const string& city, const ArcCity& newArc);
	bool addArcList(const string& file, const int& num);
	bool delArc(const string& city, const ArcCity& arc);
	const multimap<string, ArcCity>& getCityMap();
	const unordered_set<string>& getCitySet();
	//...
private:
	multimap<string, ArcCity> cityMap;
	unordered_set<string> citySet;
};
