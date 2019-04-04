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
	cityMap.insert({ city,newArc });
	return true;
}
bool Graph::addArcList(const string & file, const int& num) {
	string city;
	ArcCity arcCity;

	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			infile >> city >> arcCity.city >> arcCity.transportation >> arcCity.fare >> arcCity.time[0] >> arcCity.time[1];
			if (!addArc(city, arcCity))
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

const unordered_multimap<string, ArcCity>& Graph::getGraph() {
	return cityMap;
}

bool operator==(const ArcCity & a, const ArcCity & b) {
	return a.city == b.city &&
		a.transportation == b.transportation &&
		a.fare == b.fare &&
		a.time[0] == b.time[0] &&
		a.time[1] == b.time[1];
}

bool operator==(const tm & a, const tm & b) {
	return a.tm_hour == b.tm_hour &&
		a.tm_mday == b.tm_mday &&
		a.tm_min == b.tm_min &&
		a.tm_mon == b.tm_mon &&
		a.tm_wday == b.tm_wday &&
		a.tm_year == b.tm_year;
}

istream& operator>>(istream & is, tm & time) {
	is >> time.tm_year >> time.tm_mon >> time.tm_mday >> time.tm_hour >> time.tm_min;
	return is;
}
