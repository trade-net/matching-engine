#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp> // Include a JSON library like nlohmann/json
#include <order_request.pb.h>
#include <tcp_client.h>

// Function to send orders in a separate thread
void sendOrders(const std::vector<nlohmann::json>& orders, const std::string& ip, const std::string& port) {
    try {
        network::TcpClient client(ip, port);
        for (const auto& orderJson : orders) {
            OrderRequest order;
            order.set_id(orderJson["id"]);
            order.set_is_buy(orderJson["is_buy"]);
            order.set_units(orderJson["units"]);
            order.set_limit(orderJson["limit"]);
            order.set_timestamp(orderJson["timestamp"]);
            order.set_security(orderJson["security"]);

            std::string response = client.send(order.SerializeAsString());
            std::cout << "Response for Order ID " << order.id() << ": " << response << std::endl;
        }

        // Send the special signal to end the session
        std::string response = client.send(127, "");
        std::cout << "Session End Signal Response: " << static_cast<int>(response.front()) << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::string ip = "127.0.0.1";
        std::string port = "1234";

        // Read orders from a JSON file
        std::ifstream file("../src/client/orders.json"); // Adjusted path
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open orders.json");
        }

        nlohmann::json ordersJson;
        file >> ordersJson;

        // Split orders into chunks for multithreading
        size_t numThreads = 1; // Number of threads
        size_t chunkSize = (ordersJson.size() + numThreads - 1) / numThreads;
        std::vector<std::thread> threads;

        for (size_t i = 0; i < numThreads; ++i) {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, ordersJson.size());
            std::vector<nlohmann::json> chunk(ordersJson.begin() + start, ordersJson.begin() + end);

            threads.emplace_back(sendOrders, chunk, ip, port);
        }

        // Join all threads
        for (auto& thread : threads) {
            thread.join();
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
