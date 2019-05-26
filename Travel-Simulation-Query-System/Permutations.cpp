#include "Permutations.h"

int City::find_city(string& name, City* cityList) {
	for (int i = 0; i < num; ++i) {
		if (name == cityList[i].name) {
			return i;
		}
	}
	cout << "error" << endl;
	return -1;
}

City* City::Dijkstra(string& start, string& destination, enum strategy s) {
	multimap<string, ArcCity> map = timeTable->getCityMap();
	unordered_set<string> cities = timeTable->getCitySet();
	multimap<string, ArcCity>::iterator j;//Æðµãjµü´úÆ÷
	City* cityList, tempList;
	num = cities.size();
	int count = 0, temp = 0;

	cityList = new City[num];
	unordered_set<string>::iterator it = cities.begin();
	for (int i = 0; i < num; ++i) {
		cityList[i].name = *it;
		cityList[i].flag = (cityList[i].name == start) ? 1 : 0;
		cityList[i].value = (cityList[i].name == start) ? 0 : FLT_MAX;
		cityList[i].currentTime = (cityList[i].name == start) ? 0 : INT_MAX;//TODO
		it++;
	}


	switch (s) {
	case minCost:
		while (count != num) {
			count = 0;
			for (int i = 0; i < num; ++i) {
				if (cityList[i].flag == 1) {
					for (j = map.find(cityList[i].name); j != map.end() && j->first == cityList[i].name; ++j) {
						if ((j->second.fare + cityList[i].value) < cityList[find_city(j->second.city, cityList)].value && cityList[i].currentTime < j->second.time[0]) {
							temp = find_city(j->second.city, cityList);
							cityList[temp].value = j->second.fare + cityList[i].value;
							cityList[temp].flag = 1;
							cityList[temp].from = cityList[i].name;
							cityList[temp].order = j->second.transportation;
							cityList[temp].startTime = j->second.time[0];
							cityList[temp].currentTime = j->second.time[1];
						}
					}
					cityList[i].flag = 0;
				} else
					count++;
			}
		}
		break;
	case minTime:
		while (count != num) {
			count = 0;
			for (int i = 0; i < num; ++i) {
				if (cityList[i].flag == 1) {
					for (j = map.find(cityList[i].name); j != map.end() && j->first == cityList[i].name; ++j) {
						if (j->second.time[1] < cityList[find_city(j->second.city, cityList)].currentTime && cityList[i].currentTime < j->second.time[0]) {
							temp = find_city(j->second.city, cityList);
							cityList[temp].value = j->second.fare + cityList[i].value;
							cityList[temp].flag = 1;
							cityList[temp].from = cityList[i].name;
							cityList[temp].order = j->second.transportation;
							cityList[temp].startTime = j->second.time[0];
							cityList[temp].currentTime = j->second.time[1];
						}
					}
					cityList[i].flag = 0;
				} else
					count++;
			}
		}
		break;
	}

	return cityList;
}

TravelSchedule* City::Permutations(PassengerRequirements& require, TravelSchedule* schedule) {
	string st, des;
	City** travelList;
	ArcCity tmp;
	TravelSchedule* tempSchedule;
	int flag = 1, n, lable;
	int count = require.wayCities.size();
	string via[MAX_CITY];
	travelList = new City * [count + 1];
	list<string>::iterator it = require.wayCities.begin();
	list<ArcCity>::iterator iter;
	for (int i = 0; i < count; ++i) {
		via[i] = *it;
		++it;
	}
	it = require.wayCities.begin();

	while (true) {
		tempSchedule = new TravelSchedule;
		for (int i = 0; i <= count; ++i) {
			travelList[i] = Dijkstra(i == 0 ? require.departure : via[i - 1], via[i], require.strategy);
		}
		for (int i = 0; i <= count; ++i) {
			tmp.city = count == i ? require.destination : via[i];
			n = find_city(tmp.city, travelList[i]);
			tmp.fare = travelList[i][n].value;
			tempSchedule->planCost += tmp.fare;
			tmp.transportation = travelList[i][n].order;
			tmp.time[0] = travelList[i][n].startTime;
			tmp.time[1] = travelList[i][n].currentTime;
			tempSchedule->cities.push_back(tmp);
		}
		tempSchedule->planTime = tempSchedule->cities.back().time[1];
		lable = 0;
		switch (require.strategy) {
		case minCost:
			if (tempSchedule->planCost < schedule->planCost) {
				lable = 1;
			}
			break;
		case minTime:
			if (tempSchedule->planTime < schedule->planTime) {
				lable = 1;
			}
			schedule->planTime = travelList[count][find_city(require.destination, travelList[count])].currentTime;
			break;
		case limitedTime:
			if (tempSchedule->planTime <= require.timeLimit && tempSchedule->planCost < schedule->planCost) {
				lable = 1;
			}
		}
		if (lable) {
			iter = tempSchedule->cities.begin();
			schedule->cities.clear();
			for (size_t i = 0; i < tempSchedule->cities.size(); ++i) {
				schedule->cities.push_back(*iter);
				++iter;
			}
			schedule->planCost = tempSchedule->planCost;
			schedule->planTime = tempSchedule->planTime;
		}

		if (!flag)
			break;
		flag = prev_permutation(via, via + count);
	}

	delete tempSchedule;
	for (int i = 0; i < count + 1; ++i) {
		delete[] travelList[i];
	}
	delete[] travelList;

	return schedule;
}