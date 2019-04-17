#include "Graph.h"

Graph::Graph() {
}
Graph::Graph(const string& file, const int& num) {
	addArcList(file, num);
}
Graph::~Graph() {
}

bool Graph::addArc(const string& city, const ArcCity& newArc) {
	for (auto i = cityMap.equal_range(city); i.first != i.second; ++i.first)
		if (i.first->second == newArc)
			return false;
	citySet.insert(city);
	citySet.insert(newArc.city);
	cityMap.insert({ city,newArc });
	return true;
}

bool Graph::addArcList(const string & file, const int& num) {
	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			string* city = new string;
			ArcCity* arcCity = new ArcCity;
			infile >> *city >> arcCity->city >> arcCity->transportation >> arcCity->fare;
			for (int i = 0; i < 2; ++i) {
				tm tm_ = { 0 };
				int tmp;
				infile >> tmp;
				tm_.tm_year = tmp - 1900;
				infile >> tmp;
				tm_.tm_mon = tmp - 1;
				infile >> tm_.tm_mday >> tm_.tm_hour >> tm_.tm_min >> tm_.tm_sec;
				arcCity->time[i] = mktime(&tm_);
			}
			if (!addArc(*city, *arcCity))
				return false;
		}
	infile.close();
	return true;
}

bool Graph::delArc(const string & city, const ArcCity & arc) {
	for (auto i = cityMap.equal_range(city); i.first != i.second; ++i.first)
		if (i.first->second == arc) {
			cityMap.erase(i.first);
			return true;
		}
	return false;
}

const unordered_set<string>& Graph::getCitySet() {
	return citySet;
}

const unordered_multimap<string, ArcCity>& Graph::getCityMap() {
	return cityMap;
}

bool operator==(const ArcCity & a, const ArcCity & b) {
	return a.city == b.city &&
		a.transportation == b.transportation &&
		a.fare == b.fare &&
		a.time[0] == b.time[0] &&
		a.time[1] == b.time[1];
}

ostream& operator<<(ostream & os, ArcCity & arcCity) {
	os << arcCity.city << '\t' << arcCity.transportation << '\t' << arcCity.fare;
	for (int i = 0; i < 2; ++i) {
		os << '\t';
		char tmp[64];
		tm ltm;
		localtime_s(&ltm, &arcCity.time[i]);
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &ltm);
		os << tmp;
	}
	return os;
}

