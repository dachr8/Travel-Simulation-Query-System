#include "Simple-WebSocket-Server/server_ws.hpp"

#include "nlohmann/json.hpp"

#include "transmission_protocol.h"

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using json = nlohmann::json;

int handle_message(const std::string& in, std::string& back) {

    json j = json::parse(in);

    auto function = j["function"];

    if ("" == function) {

    }

    return 0;
}

int main() {
    WsServer server;
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


    // Start WS-server
    server.start();

}