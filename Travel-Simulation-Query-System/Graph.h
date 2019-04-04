#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class ArcCity {
public:
	string city;
	string transportation;
	float fare;
	tm time[2] = {0};
};

bool operator==(const ArcCity& a, const ArcCity& b);
bool operator==(const tm& a, const tm& b);
istream& operator>>(istream& is, tm& time);

class Graph {
public:
	Graph();
	Graph(const string& file, const int& num);
	~Graph();
	bool addArc(const string& city, const ArcCity& newArc);
	bool addArcList(const string& file, const int& num);
	bool delArc(const string& city, const ArcCity& arc);
	const unordered_multimap<string, ArcCity>& getGraph();
	//...
private:
	unordered_multimap<string, ArcCity> cityMap;
};
