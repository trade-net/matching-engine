#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <order_request.pb.h>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& server, const std::string& port)
        : io_context_(io_context), socket_(io_context) {
        tcp::resolver resolver(io_context_);
        tcp::resolver::results_type endpoints = resolver.resolve(server, port);
        connect(endpoints);
    }

    void sendMessage(const std::string& message) {
        boost::system::error_code ec;

        // Send the message to the server
        boost::asio::write(socket_, boost::asio::buffer(message), ec);
        if (!ec) {
            std::cout << "Message sent: " << message << std::endl;
        } else {
            std::cerr << "Error sending message: " << ec.message() << std::endl;
        }

        // Wait for the server's response
        std::array<char, 1024> response;
        size_t length = socket_.read_some(boost::asio::buffer(response), ec);
        if (!ec) {
            std::cout << "Server response: " << std::string(response.data(), length) << std::endl;
        } else {
            std::cerr << "Error receiving response: " << ec.message() << std::endl;
        }
    }

private:
    void connect(const tcp::resolver::results_type& endpoints) {
        boost::system::error_code ec;
        boost::asio::connect(socket_, endpoints, ec);
        if (!ec) {
            std::cout << "Connected to server!" << std::endl;
        } else {
            std::cerr << "Connection failed: " << ec.message() << std::endl;
        }
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a client and connect to server running on localhost at port 12345
        Client client(io_context, "127.0.0.1", "1234");

        // Send messages and wait for responses
		OrderRequest order;
		order.set_id(1);
		order.set_is_buy(true);
		order.set_units(10);
		order.set_limit(100);
		order.set_timestamp(1);
		order.set_security("AAPL");

        client.sendMessage(order.SerializeAsString());

        order.set_id(2);
        order.set_units(2);
        order.set_timestamp(2);
        client.sendMessage(order.SerializeAsString());

        order.set_id(3);
        order.set_units(8);
        order.set_is_buy(false);
        order.set_timestamp(3);
        client.sendMessage(order.SerializeAsString());

        order.set_id(4);
        order.set_units(7);
        order.set_timestamp(4);
        client.sendMessage(order.SerializeAsString());

        order.set_security("TSLA");
        client.sendMessage(order.SerializeAsString());
        client.sendMessage("\n");
        
        // You can continue sending more messages without closing the connection
        // client.sendMessage("Another message");

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

