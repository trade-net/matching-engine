#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <order_request.pb.h>
#include <tcp_client.h>

namespace {

struct Options {
    std::string ip = "127.0.0.1";
    std::string port = "1234";
    std::string ordersPath;      // empty: search a few default locations
    size_t numThreads = 1;
    unsigned delayMs = 0;        // pause after each order, so a demo is watchable
};

struct Stats {
    std::atomic<size_t> accepted{0};
    std::atomic<size_t> rejected{0};
};

const bool useColour = isatty(STDOUT_FILENO);

std::string paint(const char* code, const std::string& text) {
    return useColour ? std::string("\033[") + code + "m" + text + "\033[0m" : text;
}

std::mutex outputMutex;

void printHeader() {
    std::cout << paint("1", "   #   ID  SIDE  SYMBOL      QTY    PRICE   RESULT      RTT") << "\n"
              << std::string(62, '-') << std::endl;
}

void printRow(size_t index, const OrderRequest& order, bool accepted, const std::string& detail,
              std::chrono::microseconds rtt) {
    std::string side = order.is_buy() ? paint("32", "BUY ") : paint("31", "SELL");
    std::string price = order.is_market() ? "MKT" : std::to_string(order.limit());
    std::string result = accepted ? paint("32", "ACCEPTED") : paint("31", "REJECTED");

    std::lock_guard<std::mutex> lock(outputMutex);
    // both words are padded to the same width before colouring, so columns stay aligned
    std::cout << std::right << std::setw(4) << index << std::setw(5) << order.id() << "  " << side << "  "
              << std::left << std::setw(6) << order.security() << std::right
              << std::setw(7) << order.units() << std::setw(9) << price << "   " << result;
    std::cout << std::right << std::setw(9) << rtt.count() << " us";
    if (!detail.empty()) {
        std::cout << "  (" << detail << ")";
    }
    std::cout << std::endl;
}

OrderRequest toRequest(const nlohmann::json& orderJson) {
    OrderRequest order;
    order.set_id(orderJson["id"]);
    order.set_is_buy(orderJson["is_buy"]);
    order.set_units(orderJson["units"]);
    order.set_limit(orderJson["limit"]);
    order.set_timestamp(orderJson["timestamp"]);
    order.set_security(orderJson["security"]);
    order.set_is_market(orderJson.value("is_market", false));
    return order;
}

void sendOrders(const std::vector<nlohmann::json>& orders, size_t firstIndex, const Options& options, Stats& stats) {
    try {
        network::TcpClient client(options.ip, options.port);

        for (size_t i = 0; i < orders.size(); ++i) {
            OrderRequest order = toRequest(orders[i]);

            auto start = std::chrono::steady_clock::now();
            try {
                client.send(order.SerializeAsString());
                auto rtt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
                ++stats.accepted;
                printRow(firstIndex + i, order, true, "", rtt);
            } catch (const network::RequestError& e) {
                auto rtt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
                ++stats.rejected;
                printRow(firstIndex + i, order, false, e.what(), rtt);
            }

            if (options.delayMs > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(options.delayMs));
            }
        }

        // tell the server this session is finished
        client.send(127, "");

    } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cerr << paint("31", std::string("Connection error: ") + e.what()) << std::endl;
    }
}

bool loadOrders(const Options& options, nlohmann::json& ordersJson, std::string& usedPath) {
    std::vector<std::string> candidates;
    if (!options.ordersPath.empty()) {
        candidates.push_back(options.ordersPath);
    } else {
        // the binary's directory (orders.json is copied there at build time), then the source tree
        candidates = {"orders.json", "src/client/orders.json", "../src/client/orders.json"};
    }

    for (const auto& path : candidates) {
        std::ifstream file(path);
        if (file.is_open()) {
            file >> ordersJson;
            usedPath = path;
            return true;
        }
    }
    return false;
}

void printUsage(const char* program) {
    std::cout << "usage: " << program << " [--orders FILE] [--delay-ms N] [--threads N] [--host IP] [--port PORT]\n"
              << "  --orders    JSON file of orders (default: orders.json next to the binary)\n"
              << "  --delay-ms  pause after each order so the book can be watched changing (default 0)\n"
              << "  --threads   parallel connections, orders are split between them (default 1)\n";
}

} // namespace

int main(int argc, char** argv) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto next = [&]() -> std::string { return i + 1 < argc ? argv[++i] : ""; };

        if (arg == "--orders") options.ordersPath = next();
        else if (arg == "--delay-ms") options.delayMs = std::strtoul(next().c_str(), nullptr, 10);
        else if (arg == "--threads") options.numThreads = std::max<size_t>(1, std::strtoul(next().c_str(), nullptr, 10));
        else if (arg == "--host") options.ip = next();
        else if (arg == "--port") options.port = next();
        else { printUsage(argv[0]); return arg == "--help" ? 0 : 1; }
    }

    try {
        nlohmann::json ordersJson;
        std::string usedPath;
        if (!loadOrders(options, ordersJson, usedPath)) {
            std::cerr << "Failed to open orders file (try --orders FILE)" << std::endl;
            return 1;
        }

        std::cout << "Sending " << ordersJson.size() << " orders from " << usedPath << " to "
                  << options.ip << ":" << options.port << " over " << options.numThreads << " connection(s)\n"
                  << std::endl;
        printHeader();

        Stats stats;
        auto start = std::chrono::steady_clock::now();

        // split the orders into one contiguous chunk per connection
        size_t chunkSize = (ordersJson.size() + options.numThreads - 1) / options.numThreads;
        std::vector<std::thread> threads;
        for (size_t i = 0; i < options.numThreads; ++i) {
            size_t first = i * chunkSize;
            size_t last = std::min(first + chunkSize, ordersJson.size());
            if (first >= last) break;

            std::vector<nlohmann::json> chunk(ordersJson.begin() + first, ordersJson.begin() + last);
            threads.emplace_back([chunk = std::move(chunk), first, &options, &stats] {
                sendOrders(chunk, first + 1, options, stats);
            });
        }
        for (auto& thread : threads) thread.join();

        double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        size_t total = stats.accepted + stats.rejected;

        std::cout << std::string(62, '-') << "\n"
                  << total << " sent, " << paint("32", std::to_string(stats.accepted.load()) + " accepted") << ", "
                  << paint(stats.rejected ? "31" : "0", std::to_string(stats.rejected.load()) + " rejected")
                  << " in " << std::fixed << std::setprecision(1) << seconds * 1000 << " ms";
        if (options.delayMs == 0 && seconds > 0) {
            std::cout << " (" << static_cast<uint64_t>(total / seconds) << " orders/sec)";
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
