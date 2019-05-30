#include "Permutations.h"

#include <cfloat>
#include <climits>

int City::find_city(string& name) {
	auto iter = timeTable->getCitySet().begin();
	for (int i = 0; i < num; ++i) {
		if (*iter == name) {
			return i;
		}
		if(i < num - 1)
			++iter;
	}
	cout <<name<< "find error" << endl;
	return -1;
}

City* City::Dijkstra(string& start, enum strategy s, time_t start_time) {
	multimap<string, ArcCity> map = timeTable->getCityMap();
	unordered_set<string> cities = timeTable->getCitySet();
	multimap<string, ArcCity>::iterator j;
	City* cityList, tempList;
	num = cities.size();
	int count = 0, temp = 0;

	cityList = new City[num];
	unordered_set<string>::iterator it = cities.begin();
	for (int i = 0; i < num; ++i) {
		cityList[i].name = *it;
		cityList[i].flag = (cityList[i].name == start) ? 1 : 0;
		cityList[i].until = (cityList[i].name == start) ? 0 : FLT_MAX;
		cityList[i].value = 0;
		cityList[i].startTime = (cityList[i].name == start) ? 0 : INT64_MAX;
		cityList[i].currentTime = (cityList[i].name == start) ? start_time : INT64_MAX;
		it++;
	}

	
	switch (s) {
	case minCost:
		while (count != num) {
			count = 0;
			for (int i = 0; i < num; ++i) {
				if (cityList[i].flag == 1) {
					for (j = map.find(cityList[i].name); j != map.end() && j->first == cityList[i].name; ++j) {
						if ((j->second.fare + cityList[i].until) < cityList[find_city(j->second.city)].until && cityList[i].currentTime <= j->second.time[0]) {
							temp = find_city(j->second.city);
							cityList[temp].value = j->second.fare;
							cityList[temp].until = j->second.fare + cityList[i].until;
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
	case limitedTime:
	case minTime:
		while (count != num) {
			count = 0;
			for (int i = 0; i < num; ++i) {
				if (cityList[i].flag == 1) {
					for (j = map.find(cityList[i].name); j != map.end() && j->first == cityList[i].name; ++j) {
						if (j->second.time[1] < cityList[find_city(j->second.city)].currentTime && cityList[i].currentTime <= j->second.time[0]) {
							temp = find_city(j->second.city);
							cityList[temp].value = j->second.fare;
							cityList[temp].until = j->second.fare + cityList[i].until;
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
	default:
		break;
	}

	return cityList;
}

TravelSchedule* City::Permutations(PassengerRequirements& require) {
	TravelSchedule* schedule = new TravelSchedule;
	City c;
	schedule->departure = require.departure;
	schedule->destination = require.destination;
	schedule->planCost = FLT_MAX;
	schedule->planTime = INT64_MAX;

	string st, end;
	City** travelList;
	ArcCity tmp;
	TravelSchedule* tempSchedule;
	TravelSchedule* container;
	int flag = 1, s, e, lable;
	int count = require.wayCities.size();
	num = timeTable->getCitySet().size();
	string via[MAX_CITY];
    auto itt = timeTable->getCitySet().begin();
	list<string>::iterator it;
	list<ArcCity>::iterator iter;
	for (int i = 0, c = 1; flag; ++c) {
		it = require.wayCities.begin();
		for (int j = 0; j < count; ++j) {
			if (*itt == *it) {
				via[i] = *itt;
				++i;
			}
			++it;
		}
		if (c < num)
			++itt;
		else
			break;
	}

	time_t start_time;
	while (true) {
		travelList = new City * [num];
		for (int i = 0; i < num; ++i) {
			travelList[i] = nullptr;
		}
		itt = timeTable->getCitySet().begin();
		st = *itt;
		tempSchedule = new TravelSchedule;
		start_time = require.timeStart;
		
		for (int i = 0; i <= count; ++i) {
			container = new TravelSchedule;
			st = i == 0 ? require.departure : via[i - 1];
			end = i == count ? require.destination : via[i];
			s = find_city(st);
			e = find_city(end);
			travelList[s] = Dijkstra(st, require.strategy, start_time);
			int k = 0;
			for (int f = 1; f;) {
				if (travelList[s][e].from == st) {
					f = 0;
				}
				if (travelList[s][e].value == FLT_MAX || travelList[s][e].from == "") {
					goto skip;
				}
				
				if (!k) {
					tempSchedule->planCost += travelList[s][e].until;
					start_time = travelList[s][e].currentTime;
				}
				if (i == count && end == require.destination) {
					tempSchedule->planTime = travelList[s][e].currentTime;
				}

				tmp.transportation = travelList[s][e].order;


				tmp.city = end;
				tmp.fare = travelList[s][e].value;
				tmp.time[0] = travelList[s][e].startTime;
				tmp.time[1] = travelList[s][e].currentTime;
				container->cities.push_back(tmp);
				if (f) {
					end = travelList[s][e].from;
					e = find_city(end);
				}
				++k;
			}
			iter = container->cities.end();
			--iter;
			while (k) {
				tempSchedule->cities.push_back(*iter);
				if(k>1)
					--iter;
				--k;
			}
			
			delete container;
		}
		

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
			break;
		case limitedTime:
			if (tempSchedule->planTime <= require.timeLimit && tempSchedule->planCost < schedule->planCost) {
				lable = 1;
			}
		}
		if (lable) {
			iter = tempSchedule->cities.begin();
			schedule->cities.clear();
			for (int i = 0; i < tempSchedule->cities.size(); ++i) {
				schedule->cities.push_back(*iter);
				++iter;
			}
			schedule->planCost = tempSchedule->planCost;
			schedule->planTime = tempSchedule->planTime;
		}

	skip:

		delete tempSchedule;
		for (int i = 0; i < num; ++i) {
			if (travelList[i] != nullptr) {
				delete[] travelList[i];
			}
		}
		delete[] travelList;
		
		if (!flag)
			break;
		flag = prev_permutation(via, via + count);
	}
	if (schedule->cities.size() <= count) {
		schedule->status.currentStatus = error;
	}

	

	return schedule;
}
