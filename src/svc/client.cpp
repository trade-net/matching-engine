#include <iostream>
#include <string>
#include <order_request.pb.h>
#include <tcp_client.h>

int main() {
    try {
    	std::string ip = "127.0.0.1";
    	std::string port = "1234";
    	network::TcpClient client(ip, port);
    	std::string response;

        // Send messages and wait for responses
		OrderRequest order;
		order.set_id(1);
		order.set_is_buy(true);
		order.set_units(10);
		order.set_limit(100);
		order.set_timestamp(1);
		order.set_security("AAPL");

        response = client.send(0, order.SerializeAsString());
        std::cout << response << std::endl;

        order.set_id(2);
        order.set_units(2);
        order.set_timestamp(2);
        response = client.send(0, order.SerializeAsString());
        std::cout << response << std::endl;

        order.set_id(3);
        order.set_units(8);
        order.set_is_buy(false);
        order.set_timestamp(3);
        response = client.send(0, order.SerializeAsString());
        std::cout << response << std::endl;

        order.set_id(4);
        order.set_units(7);
        order.set_timestamp(4);
        response = client.send(0, order.SerializeAsString());
        std::cout << response << std::endl;

        order.set_security("TSLA");
        response = client.send(0, order.SerializeAsString());
        std::cout << response << std::endl;

        response = client.send(127, "");
        std::cout << response << std::endl;
        
        // You can continue sending more messages without closing the connection
        // client.sendMessage("Another message");

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

