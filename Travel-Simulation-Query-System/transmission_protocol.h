#pragma once
#ifndef CPP_BACKEND_TRANSMISSION_PROTOCOL_H

#define CPP_BACKEND_TRANSMISSION_PROTOCOL_H


#include "PassengerTable.h"
#include <vector>


namespace fdt {
    /**

     * Vertex node for transmission

     * use display_name to select distinct vertex instance

     */

    class Vertex {

    private:

        // unsigned id;

        std::string display_name;

    public:

        Vertex() {}

        Vertex(const std::string &display_name) : display_name(display_name) {}

        Vertex(const Vertex &_vertex) : display_name(_vertex.display_name) {}

        std::string get_display_name();

    };


    /**

     * Using edge to draw edges on map

     */

    class Edge {

    private:



        //unsigned id;



        std::string source_vertex_display_name;

        std::string destination_vertex_display_name;


        unsigned length; // optional

    public:

        Edge() {}

        Edge(const Edge &_edge) : source_vertex_display_name(_edge.source_vertex_display_name),
                                  destination_vertex_display_name(_edge.destination_vertex_display_name),
                                  length(_edge.length) {}


        Edge(std::string source_vertex_display_name, std::string destination_vertex_display_name) :

                source_vertex_display_name(source_vertex_display_name),
                destination_vertex_display_name(destination_vertex_display_name), length(200) {}


        std::string get_source_vertex_display_name();


        std::string get_destination_vertex_display_name();


        unsigned get_length();

    };


    /**

     * Passing an instance of PassengerRequirement

     * for backend to compute

     */

    class PassengerRequirement {

    private:

        std::string from_vertex_display_name;

        std::string to_vertex_display_name;


        time_t start_time;

        std::vector<std::string> pass_by_vertex_display_name_vector;


        strategy travel_strategy;

        time_t total_time_limit;

    public:

        PassengerRequirement() {}

        PassengerRequirement(const PassengerRequirement &_requirement) :
                from_vertex_display_name(_requirement.from_vertex_display_name),
                to_vertex_display_name(_requirement.to_vertex_display_name),
                start_time(_requirement.start_time),
                pass_by_vertex_display_name_vector(_requirement.pass_by_vertex_display_name_vector),
                travel_strategy(_requirement.travel_strategy),
                total_time_limit(_requirement.total_time_limit) {}

        PassengerRequirement(std::string from_vertex_display_name,

                             std::string to_vertex_display_name,

                             time_t start_time,

                             std::vector<std::string> pass_by_vertex_display_name_vector,

                             strategy travel_strategy,

                             time_t total_time_limit) :

                from_vertex_display_name(from_vertex_display_name),

                to_vertex_display_name(to_vertex_display_name),

                start_time(start_time),

                pass_by_vertex_display_name_vector(pass_by_vertex_display_name_vector),

                travel_strategy(travel_strategy),

                total_time_limit(total_time_limit) {}


        std::string get_from_vertex_display_name();


        std::string get_to_vertex_display_name();

        time_t get_start_time();

        std::vector<std::string> get_pass_by_vertex_display_name_vector();

        strategy get_travel_strategy();

        time_t get_total_time_limit();

    };


    /**

     * Result of computing

     */

    class PlanSingleTransportation {

    private:



        // from and to can be the same

        std::string from_vertex_display_name;

        std::string to_vertex_display_name;


        time_t start_time;

        time_t end_time;


        std::string transportation_display_info;

    public:

        PlanSingleTransportation() {}

        PlanSingleTransportation(const PlanSingleTransportation &_planSingleTransportation) :
                from_vertex_display_name(_planSingleTransportation.from_vertex_display_name),
                to_vertex_display_name(_planSingleTransportation.to_vertex_display_name),
                start_time(_planSingleTransportation.start_time),
                end_time(_planSingleTransportation.end_time),
                transportation_display_info(_planSingleTransportation.transportation_display_info) {}

        PlanSingleTransportation(std::string from_vertex_display_name,

                                 std::string to_vertex_display_name,

                                 time_t start_time,

                                 time_t end_time,

                                 std::string transportation_display_info) :

                from_vertex_display_name(from_vertex_display_name),

                to_vertex_display_name(to_vertex_display_name),

                start_time(start_time),

                end_time(end_time) {}


        std::string get_from_vertex_display_name();


        std::string get_to_vertex_display_name();


        time_t get_start_time();

        time_t get_end_time();


        std::string get_transportation_display_info();

    };


    class Passenger {

    private:

        std::string id;

    public:

        Passenger() {}

        Passenger(const Passenger &_passenger) : id(_passenger.id) {}

        Passenger(std::string id) : id(id) {}


        std::string get_id();

    };


    class TotalTransportationPlan {

    private:

        std::vector<PlanSingleTransportation> single_transportation_plan_vector;

        Passenger passenger;

        std::string display_info;


    public:

        TotalTransportationPlan() {}

        TotalTransportationPlan(const TotalTransportationPlan &_plan) :
                single_transportation_plan_vector(_plan.single_transportation_plan_vector),
                passenger(_plan.passenger),
                display_info(_plan.display_info) {}

        TotalTransportationPlan(std::vector<PlanSingleTransportation> single_transportation_plan_vector,

                                Passenger passenger,

                                std::string display_info) : single_transportation_plan_vector(
                single_transportation_plan_vector),

                                                            passenger(passenger), display_info(display_info) {}


        std::vector<PlanSingleTransportation> get_single_transportation_plan_vector();


        Passenger get_passenger();


        std::string get_display_info();

    };


    std::vector<Vertex> get_all_vertex();


    std::vector<Edge> get_all_display_edge();


    TotalTransportationPlan submit_passenger_requirement(Passenger passenger, PassengerRequirement requirement);

    std::vector<TotalTransportationPlan> sync_total_transportation_plans();

    time_t sync_time();

}


#endif //CPP_BACKEND_TRANSMISSION_PROTOCOL_H