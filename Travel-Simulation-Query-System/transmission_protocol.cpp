#include"transmission_protocol.h"
#include"Permutations.h"

#define plus 1000


namespace fdt {
	std::vector<TotalTransportationPlan> allplan;

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
		TravelSchedule schedule;
		auto iter = pass.begin();
		require.departure = requirement.get_from_vertex_display_name();
		require.destination = requirement.get_to_vertex_display_name();
		require.strategy = requirement.get_travel_strategy();
		require.timeLimit = requirement.get_total_time_limit() / plus;
		require.timeStart = requirement.get_start_time() / plus;
		while (iter != pass.end()) {
			require.wayCities.push_back(*iter);
			++iter;
		}
		//City c;
			//schedule = c.Permutations(require);
		passengers->addPassenger(passenger.get_id(), require);
		schedule = passengers->generateTravelSchedule(passenger.get_id());
		passengers->printTravelSchedule(passenger.get_id());

		if (schedule.status.currentStatus == error) {
			TotalTransportationPlan emptyplan(plan, passenger, "error");
			return emptyplan;
		} else {
			time_t current_time = requirement.get_start_time() / plus;
			list<ArcCity>::iterator iter = schedule.cities.begin();

			std::string st, end;
			st = requirement.get_from_vertex_display_name();
			while (iter != schedule.cities.end()) {
				std::string single_display;
				end = iter->city;
				if (current_time < iter->time[0]) {
					single_display += "rest";
					single_display += " ";
					single_display += "0";
					PlanSingleTransportation singlePlan(st, st, current_time * plus, iter->time[0] * plus, single_display);
					plan.push_back(singlePlan);
					current_time = iter->time[0];
				} else {
					single_display += iter->transportation;
					single_display += " ";
					single_display += std::to_string(iter->fare);
					PlanSingleTransportation singlePlan(st, end, iter->time[0] * plus, iter->time[1] * plus, single_display);
					plan.push_back(singlePlan);
					current_time = iter->time[1];
					st = end;
					++iter;
				}
			}
		}
		string display = "";


     display +=  std::to_string(schedule.planCost);
		 display += " ";
		 display +=  std::to_string(schedule.planTime * plus);


		TotalTransportationPlan totalplan(plan, passenger, display);
		allplan.push_back(totalplan);

		return totalplan;
	}

	std::vector<TotalTransportationPlan> sync_total_transportation_plans() {
		return allplan;
	}

	time_t sync_time() {
		return now * 1000;
	}

}
