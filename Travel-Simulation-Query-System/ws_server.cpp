#include "Simple-WebSocket-Server/server_ws.hpp"

#include "nlohmann/json.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>

#include "transmission_protocol.h"

#include "ws_server.h"

namespace fdt {

    using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
    using json = nlohmann::json;

    std::thread server_thread;
    WsServer server;

    long long get_system_timemill() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    int handle_message(const std::string &in, std::string &back) {

        json j = json::parse(in);

        auto function = j["function"];

        if (function == "get_all_vertex") {

            std::vector<fdt::Vertex> all_vertex = fdt::get_all_vertex();

            json json_array = json::array();

            for (auto it = all_vertex.begin(); it != all_vertex.end(); ++it) {

                json vertx = json::object();
                vertx["id"] = it->get_display_name();

//                std::cout << "vertex_display_name" << it->get_display_name() << std::endl;
                json_array.emplace_back(vertx);
            }

//            std::cout << "vertex_json" << json_array.dump() << std::endl;

            json r_json;

            r_json["function"] = "get_all_vertex";
            r_json["vertex"] = json_array;

            back = r_json.dump();

//        back = "{\"function\": \"get_all_vertex\", \"vertex\": [{\"id\": \"id1\"}, {\"id\": \"id2\"}]}";
            return 0;
        }

        if (function == "get_all_display_edge") {

            std::vector<fdt::Edge> all_edges = fdt::get_all_display_edge();

            json json_array = json::array();

            for (auto it = all_edges.begin(); it != all_edges.end(); ++it) {

                json vertx = json::object();
                vertx["source"] = it->get_source_vertex_display_name();
                vertx["target"] = it->get_destination_vertex_display_name();

                // length is omitted

                json_array.emplace_back(vertx);
            }

            json r_json;

            r_json["function"] = "get_all_display_edge";
            r_json["edges"] = json_array;

            back = r_json.dump();

//        back = "{\"function\": \"get_all_display_edge\", \"edges\": [{\"source\": \"id1\", \"target\": \"id2\"}]}";
            return 0;
        }

        if (function == "sync_time") {

//         timev = fdt::get_system_timemill();
            time_t timev = fdt::sync_time();

            back = "{\"function\": \"sync_time\", \"time\": " + std::to_string(timev) + "}";
            return 0;
        }

        if (function == "submit_passenger_requirement") {

            std::cout << "submit_passenger_requirement_function_called" << std::endl;

            fdt::Passenger passenger = fdt::Passenger(j["passenger"]);

            std::vector<std::string> pass_by_vertex = std::vector<std::string>();

            strategy s;
            if (j["travel_strategy"] == "min_cost") {
                s = minCost;
            } else
            if (j["travel_strategy"] == "min_time") {
                s = minTime;
            } else
            if (j["travel_strategy"] == "limited_time") {
                s = limitedTime;
            }

            for (auto it = j["pass_by_vertex_display_name"].begin(); it != j["pass_by_vertex_display_name"].end(); ++it) {
                pass_by_vertex.emplace_back(*it);
            }

            fdt::PassengerRequirement requirement = fdt::PassengerRequirement(j["from"], j["to"], j["start_time"], pass_by_vertex, s, j["total_time_limit"]);

            fdt::TotalTransportationPlan plan = fdt::submit_passenger_requirement(passenger, requirement);

            json total_transportation_plan;

            total_transportation_plan["display_info"] = plan.get_display_info();

            total_transportation_plan["passenger"] = plan.get_passenger().get_id();

            json json_array = json::array();

            std::vector<PlanSingleTransportation> v = plan.get_single_transportation_plan_vector();

            for (auto it = v.begin(); it != v.end(); ++it) {

                std::cout << "it value " << it->get_from_vertex_display_name() << ", " <<
                          it->get_to_vertex_display_name() << ", " << it->get_start_time() << ", "<<
                          it->get_end_time() << std::endl;

                json single_plan;
                single_plan["from"] = it->get_from_vertex_display_name();
                single_plan["to"] = it->get_to_vertex_display_name();
                single_plan["start_time"] = it->get_start_time();
                single_plan["end_time"] = it->get_end_time();

                std::cout << single_plan.dump() << std::endl;

                json_array.emplace_back(single_plan);
            }

            total_transportation_plan["single_transportation_plans"] = json_array;

            json r_json;
            r_json["total_transportation_plan"] = total_transportation_plan;

            r_json["function"] = "submit_passenger_requirement";


            std::cout << r_json.dump() << std::endl;

            back = r_json.dump();

//        long long timev = fdt::get_system_timemill();

//        back = "{"
//                "\"function\": \"submit_passenger_requirement\", "
//                "\"total_transportation_plan\": {"
//                    " \"passenger\": \"Mark_A\", "
//                    "\"display_info\": \"display_info\", "
//                    "\"single_transportation_plans\": [ "
//                        "{"
//                            "\"from\": \"id1\", "
//                            "\"to\": \"id2\", "
//                            "\"start_time\": " + std::to_string(timev + 1000) + ", "
//                            "\"end_time\": " + std::to_string(timev + 6000) +
//                        "}, "
//                        "{"
//                            "\"from\": \"id2\", "
//                            "\"to\": \"id1\", "
//                            "\"start_time\": " + std::to_string(timev + 6000) + ", "
//                            "\"end_time\": " + std::to_string(timev + 11000) +
//                        "}"
//                    "]"
//                "}"
//                "}";
        }

        return 0;
    }

    int start_ws_server() {
        server.config.port = 8145;

        // Example 1: echo WebSocket endpoint
        // Added debug messages for example use of the callbacks
        // Test with the following JavaScript:
        //   var ws=new WebSocket("ws://localhost:8080/echo");
        //   ws.onmessage=function(evt){console.log(evt.data);};
        //   ws.send("test");
        auto &echo = server.endpoint["^/?$"];

        echo.on_message = [](std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> message) {
            std::string message_str = message->string();

            std::cout << "Server: Message received: \"" << message_str << "\" from " << connection.get() << std::endl;

            std::string r_message_str;

            handle_message(message_str, r_message_str);

            std::cout << "Server: Sending message \"" << r_message_str << "\" to " << connection.get() << std::endl;

            auto send_stream = std::make_shared<WsServer::SendStream>();
            *send_stream << r_message_str;
            // connection->send is an asynchronous function
            connection->send(send_stream, [](const SimpleWeb::error_code &ec) {
                if (ec) {
                    std::cout << "Server: Error sending message. " <<
                              // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                              "Error: " << ec << ", error message: " << ec.message() << std::endl;
                }
            });
        };

        echo.on_open = [](std::shared_ptr<WsServer::Connection> connection) {
            std::cout << "Server: Opened connection " << connection.get() << std::endl;
        };

        // See RFC 6455 7.4.1. for status codes
        echo.on_close = [](std::shared_ptr<WsServer::Connection> connection, int status, const std::string & /*reason*/) {
            std::cout << "Server: Closed connection " << connection.get() << " with status code " << status << std::endl;
        };

        // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
        echo.on_error = [](std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
            std::cout << "Server: Error in connection " << connection.get() << ". "
                      << "Error: " << ec << ", error message: " << ec.message() << std::endl;
        };


        server_thread = std::thread([&server]() {
            // Start WS-server
            server.start();
        });

        server_thread.detach();

        return 0;
    }
}