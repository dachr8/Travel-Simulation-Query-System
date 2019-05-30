#include "TimeTable.h"

TimeTable::TimeTable() {
}
TimeTable::TimeTable(const string& file, const int& num) {
	addArcList(file, num);
}
TimeTable::~TimeTable() {
}

bool TimeTable::addArc(const string& city, const ArcCity& newArc) {
	for (auto i = cityMap.equal_range(city); i.first != i.second; ++i.first)
		if (i.first->second == newArc)
			return false;
	citySet.insert(city);
	citySet.insert(newArc.city);
	cityMap.insert({ city,newArc });
	return true;
}

bool TimeTable::addArcList(const string& file, const int& num) {
	ifstream infile(file);
	if (infile) {

		// skip UTF-8 BOM header for Windows saved files
		char c[3];
		for (int j = 0; j < 3; ++j) {
			c[j] = infile.get();
		}
		if (!(c[0] == 0xef && c[1] == 0xbb && c[2] == 0xbf)) {
			for (int j = 0; j < 3; ++j) {
				infile.unget();
			}
		}

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
				infile >> tm_.tm_mday >> tm_.tm_hour;
				tm_.tm_min = 0;
				tm_.tm_sec = 0;
				arcCity->time[i] = mktime(&tm_);
			}
			if (!addArc(*city, *arcCity))
				return false;
		}
	}
		
	infile.close();
	return true;
}

bool TimeTable::delArc(const string& city, const ArcCity& arc) {
	for (auto i = cityMap.equal_range(city); i.first != i.second; ++i.first)
		if (i.first->second == arc) {
			cityMap.erase(i.first);
			return true;
		}
	return false;
}

const unordered_set<string>& TimeTable::getCitySet() {
	return citySet;
}

const multimap<string, ArcCity>& TimeTable::getCityMap() {
	return cityMap;
}

bool operator==(const ArcCity& a, const ArcCity& b) {
	return a.city == b.city &&
		a.transportation == b.transportation &&
		a.fare == b.fare &&
		a.time[0] == b.time[0] &&
		a.time[1] == b.time[1];
}


string ArcCity::toString() {
	string s = city + '\t' + transportation +  "票价：" + to_string(fare);

	for (int i = 0; i < 2; ++i) {
		s += '\t';
		char tmp[64];
		tm ltm;

#ifdef _WIN32
		localtime_s(&ltm, &time[i]);
#endif
#ifdef __linux__
		localtime_r(&time[i], &ltm);
#endif

		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &ltm);
		s += tmp;
	}
	return s;
}
