#pragma once
#include <algorithm>
#include <string>
#include "PassengerTable.h"
#include "TimeTable.h"
namespace std {
	const int MAX_CITY = 10;
	static int num;//城市总数


	class City {
	public:
		City* Dijkstra(string& start, enum strategy s, time_t start_time);
		int find_city(string& name, City* cityList);
		TravelSchedule* Permutations(PassengerRequirements& require);
	private:
		string name;//当前名称
		string from;//来自
		string order;//航班
		time_t startTime;//从之前目的地出发时间
		time_t currentTime;//抵达时间
		int flag;//0 1 -1扫描标识
		float value;//当前
	};
}