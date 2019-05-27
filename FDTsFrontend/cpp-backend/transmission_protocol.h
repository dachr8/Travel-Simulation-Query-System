#pragma once

#ifndef CPP_BACKEND_TRANSMISSION_PROTOCOL_H
#define CPP_BACKEND_TRANSMISSION_PROTOCOL_H

#include "../../Travel-Simulation-Query-System/PasseengerTable.h"

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
        Vertex(const std::string &display_name) : display_name(display_name) {}

        std::string get_display_name();
    };

    /**
     * Using edge to draw edges on map
     */
    class Edge {
    private:

        // unsigned id;

        std::string source_vertex_display_name;
        std::string destination_vertex_display_name;

        unsigned length; // optional
    public:
        Edge(std::string source_vertex_display_name, std::string destination_vertex_display_name, unsigned length) :
                source_vertex_display_name(source_vertex_display_name), destination_vertex_display_name(destination_vertex_display_name), length(length) {}

        Edge(std::string source_vertex_display_name, std::string destination_vertex_id) :
                id(id), source_vertex_id(source_vertex_id), destination_vertex_id(destination_vertex_id), length(200) {}

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
        PassengerRequirement(std::string from_vertex_display_name,
        std::string to_vertex_display_name,
        time_t start_time,
        std::vector<std::string> pass_by_vertex_display_name_vector,
        strategy travel_strategy,
        time_t total_time_limit):
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
        Passenger(std::string id) : id(id) {}

        std::string get_id();
    };

    std::vector<Vertex> get_all_vertex();

    std::vector<Edge> get_all_display_edge();

    std::vector<PlanSingleTransportation> submit_passenger_requirement(Passenger passenger, PassengerRequirement requirement);
}

#endif //CPP_BACKEND_TRANSMISSION_PROTOCOL_H
