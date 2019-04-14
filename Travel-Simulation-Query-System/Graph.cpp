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
	ifstream infile(file);
	if (infile)
		for (int i = 0; i < num && !infile.eof(); ++i) {
			string* city = new string;
			ArcCity* arcCity = new ArcCity;
			infile >> *city >> arcCity->city >> arcCity->transportation >> arcCity->fare >> arcCity->time[0] >> arcCity->time[1];
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

istream& operator>>(istream & is, time_t & time) {
	tm tm_ = { 0 };
	is >> tm_.tm_year >> tm_.tm_mon >> tm_.tm_mday >> tm_.tm_hour >> tm_.tm_min;
	tm_.tm_year -= 1900;
	--tm_.tm_mon;
	time = mktime(&tm_);
	return is;
}
