#include"transmission_protocol.h"
#include"Permutations.h"

#define plus 1000

namespace fdt {
	std::string Vertex::get_display_name() {
		return this->display_name;
	}

	std::string Edge::get_source_vertex_display_name() {
		return this->source_vertex_display_name;
	}
	std::string Edge::get_destination_vertex_display_name() {
		return this->destination_vertex_display_name;
	}
	unsigned Edge::get_length() {
		return this->length;
	}

	std::string PassengerRequirement::get_from_vertex_display_name() {
		return this->from_vertex_display_name;
	}
	std::string PassengerRequirement::get_to_vertex_display_name() {
		return this->to_vertex_display_name;
	}
	time_t PassengerRequirement::get_start_time() {
		return this->start_time;
	}
	std::vector<std::string> PassengerRequirement::get_pass_by_vertex_display_name_vector() {
		return this->pass_by_vertex_display_name_vector;
	}
	strategy PassengerRequirement::get_travel_strategy() {
		return this->travel_strategy;
	}
	time_t PassengerRequirement::get_total_time_limit() {
		return this->total_time_limit;
	}


	std::string Passenger::get_id() {
		return this->id;
	}

	std::string PlanSingleTransportation::get_from_vertex_display_name() {
		return this->from_vertex_display_name;
	}
	std::string PlanSingleTransportation::get_to_vertex_display_name() {
		return this->to_vertex_display_name;
	}
	time_t PlanSingleTransportation::get_start_time() {
		return this->start_time;
	}
	time_t PlanSingleTransportation::get_end_time() {
		return this->end_time;
	}
	std::string PlanSingleTransportation::get_transportation_display_info() {
		return this->transportation_display_info;
	}

	Passenger TotalTransportationPlan::get_passenger() {
		return this->passenger;
	}
	std::string TotalTransportationPlan::get_display_info() {
		return this->display_info;
	}
	std::vector<PlanSingleTransportation> TotalTransportationPlan::get_single_transportation_plan_vector() {
		return this->single_transportation_plan_vector;
	}



	std::vector<Vertex> get_all_vertex() {
		std::vector<Vertex> city_vector;
		auto iter = timeTable->getCitySet().begin();
		while (iter != timeTable->getCitySet().end()) {
			Vertex city(*iter);
			city_vector.push_back(city);
			++iter;
		}

		return city_vector;
	}

	std::vector<Edge> get_all_display_edge() {
		std::vector<Edge> edge_vector;
		std::multimap<std::string, ArcCity>::const_iterator iter = timeTable->getCityMap().begin();
		int flag;
		while (iter != timeTable->getCityMap().end()) {
			auto it = edge_vector.begin();
			flag = 1;
			while (it != edge_vector.end()) {
				if ((it->get_destination_vertex_display_name() == iter->first && it->get_source_vertex_display_name() == iter->second.city)
					|| (it->get_destination_vertex_display_name() == iter->second.city && it->get_source_vertex_display_name() == iter->first)) {
					flag = 0;
					break;
				}
				++it;
			}
			if (flag) {
				Edge edge(iter->first, iter->second.city);
				edge_vector.push_back(edge);
			}
			++iter;
		}
		return edge_vector;
	}

	TotalTransportationPlan submit_passenger_requirement(Passenger passenger, PassengerRequirement requirement) {
		PassengerRequirements require;
		std::vector<PlanSingleTransportation> plan;
		std::vector<std::string> pass = requirement.get_pass_by_vertex_display_name_vector();
		TravelSchedule* schedule = new TravelSchedule;
		City c;
		auto iter = pass.begin();
		require.departure = requirement.get_from_vertex_display_name();
		require.destination = requirement.get_to_vertex_display_name();
		require.strategy = requirement.get_travel_strategy();
		require.timeLimit = requirement.get_total_time_limit()/plus;
		require.timeStart = requirement.get_start_time()/plus;
		while (iter != pass.end()) {
			require.wayCities.push_back(*iter);
			++iter;
		}
		PassengerTable user;
		//user.generateTravelSchedule
		schedule = c.Permutations(require);

		if (schedule->status.currentStatus == error) {
			TotalTransportationPlan emptyplan(plan, passenger, "error");
			return emptyplan;
		}
		else {
			time_t current_time = requirement.get_start_time()/1000;
			list<ArcCity>::iterator iter = schedule->cities.begin();
			std::string st, end;
			st = requirement.get_from_vertex_display_name();
			while (iter != schedule->cities.end()) {
				end = iter->city;
				if (current_time < iter->time[0]) {
					PlanSingleTransportation singlePlan(st, st, current_time*plus, iter->time[0]*plus, "rest");
					plan.push_back(singlePlan);
					current_time = iter->time[0];
				}
				else {
					PlanSingleTransportation singlePlan(st, end, iter->time[0]*plus, iter->time[1]*plus, iter->transportation);
					plan.push_back(singlePlan);
					current_time = iter->time[1];
					st = end;
					++iter;
				}
			}
		}
		string display = "";
		display += std::to_string(schedule->planCost);
		display += " ";
		display += std::to_string(schedule->planTime);
		TotalTransportationPlan totalplan(plan,passenger,display);

		delete schedule;
		return totalplan;
	}

    /*TotalTransportationPlan submit_passenger_requirement(Passenger passenger, PassengerRequirement requirement) {
        time_t t = sync_time();

        Passenger p("Mark_A");

		std::cout << p.get_id() << std::endl;

        std::vector<PlanSingleTransportation> singles{};

        PlanSingleTransportation s1(
                "Beijing",

                "Wuhan",

                t + 3600000,

                t + 3600000 * 5,

                "info");

		std::cout << "s1" << s1.get_from_vertex_display_name() << std::endl;

        singles.emplace_back(s1);

		std::cout << "s1" << s1.get_from_vertex_display_name() << std::endl;

        PlanSingleTransportation s2("Wuhan",

                                    "Guangzhou",

                                    t + 3600000 * 5,

                                    t + 3600000 * 10,

                                    "info2");

        singles.emplace_back(s2);

        PlanSingleTransportation s3("Guangzhou",

                                    "Tianjin",

                                    t + 3600000 * 10,

                                    t + 3600000 * 15,

                                    "info3");

        singles.emplace_back(s3);


		TotalTransportationPlan plan = TotalTransportationPlan (
                singles,
                p,
                "info");

        return plan;
    }*/

	time_t sync_time() {
		return now * 1000;
	}

}
