#include"transmission_protocol.h"
#include"Permutations.h"

#define plus 3600*1000

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
		std::unordered_set<std::string>::iterator iter = timeTable->getCitySet().begin();
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
		while (iter != timeTable->getCityMap().end()) {
			Edge edge(iter->first, iter->second.city);
			edge_vector.push_back(edge);
			++iter;
		}
		return edge_vector;
	}

	TotalTransportationPlan submit_passenger_requirement(Passenger passenger, PassengerRequirement requirement) {
		PassengerRequirements require;
		std::vector<PlanSingleTransportation> plan;

		TravelSchedule* schedule = new TravelSchedule;
		City c;
		std::vector<std::string>::iterator iter = requirement.get_pass_by_vertex_display_name_vector().begin();
		require.departure = requirement.get_from_vertex_display_name();
		require.destination = requirement.get_to_vertex_display_name();
		require.strategy = requirement.get_travel_strategy();
		require.timeLimit = requirement.get_total_time_limit();
		require.timeStart = requirement.get_start_time();
		while (iter != requirement.get_pass_by_vertex_display_name_vector().end()) {
			require.wayCities.push_back(*iter);
			++iter;
		}
		schedule = c.Permutations(require);
		if (schedule->status.currentStatus == error) {
			TotalTransportationPlan emptyplan(plan, passenger, "error");
			return emptyplan;
		}
		else {
			time_t current_time = requirement.get_start_time();
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
		display += schedule->planCost;
		display += "\t";
		display += schedule->planTime;
		TotalTransportationPlan totalplan(plan,passenger,display);

		delete schedule;
		return totalplan;
	}
}
