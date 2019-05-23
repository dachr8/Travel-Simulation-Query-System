#pragma once
#include<algorithm>
using namespace std;

const int MAX_CITY = 10;
int num;//��������


class City {
public:
	City* Dijkstra(string& start, string& destination, enum strategy s);
	int find_city(string& name, City* cityList);
	TravelSchedule* Permutations(PassengerRequirements& require, TravelSchedule* schedule);
private:
	string name;//��ǰ����
	string from;//����
	string order;//����
	time_t startTime;//��֮ǰĿ�ĵس���ʱ��
	time_t currentTime;//�ִ�ʱ��
	int flag;//0 1 -1ɨ���ʶ
	int value;//��ǰ
};