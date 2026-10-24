#include <order_book.h>
#include <order.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace{

struct Percentiles{
	double mean;
	double p50;
	double p90;
	double p99;
	double p999;
	double max;
};

Percentiles computePercentiles(std::vector<double>& samplesNs)
{
	std::sort(samplesNs.begin(), samplesNs.end());

	auto at = [&](double fraction){
		size_t idx = static_cast<size_t>(fraction * (samplesNs.size() - 1));
		return samplesNs[idx];
	};

	double sum = 0;
	for(double s : samplesNs) sum += s;

	return Percentiles{
		sum / samplesNs.size(),
		at(0.50),
		at(0.90),
		at(0.99),
		at(0.999),
		samplesNs.back()
	};
}

void printPercentiles(const std::string& label, const Percentiles& p)
{
	std::cout << label << " (ns/order):"
		<< " mean=" << p.mean
		<< " p50=" << p.p50
		<< " p90=" << p.p90
		<< " p99=" << p.p99
		<< " p99.9=" << p.p999
		<< " max=" << p.max
		<< std::endl;
}

// Generates a stream of orders clustered around a fixed mid price so the
// book accumulates realistic resting depth on both sides while still
// crossing regularly, rather than degenerating to an empty or one-sided book.
std::vector<Order> generateOrders(size_t count, int numSecurities, uint32_t seed)
{
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> sideDist(0, 1);
	std::uniform_int_distribution<int> unitsDist(1, 100);
	std::uniform_int_distribution<int> priceOffsetDist(-25, 25);
	std::uniform_int_distribution<int> securityDist(0, numSecurities - 1);

	std::vector<std::string> securities;
	for(int i = 0; i < numSecurities; ++i){
		securities.push_back("SEC" + std::to_string(i));
	}

	const int midPrice = 10000;

	std::vector<Order> orders;
	orders.reserve(count);

	for(size_t i = 0; i < count; ++i){
		bool isBuy = sideDist(rng) == 0;
		int units = unitsDist(rng);
		int price = midPrice + priceOffsetDist(rng);
		const std::string& security = securities[securityDist(rng)];
		orders.emplace_back(static_cast<int>(i), isBuy, units, price, static_cast<int>(i), security);
	}

	return orders;
}

// Measures per-order latency of OrderBook::processOrder directly, bypassing
// the network layer, threadpool dispatch, and logging entirely.
void runLatencyBenchmark(const std::vector<Order>& orders, size_t warmupCount)
{
	OrderBook book;

	for(size_t i = 0; i < warmupCount; ++i){
		book.processOrder(orders[i]);
	}

	std::vector<double> samplesNs;
	samplesNs.reserve(orders.size() - warmupCount);

	for(size_t i = warmupCount; i < orders.size(); ++i){
		auto start = std::chrono::steady_clock::now();
		book.processOrder(orders[i]);
		auto end = std::chrono::steady_clock::now();
		samplesNs.push_back(std::chrono::duration<double, std::nano>(end - start).count());
	}

	printPercentiles("Single-instrument processOrder latency", computePercentiles(samplesNs));
}

// Measures sustained orders/sec on a single instrument's book with no
// per-order timing overhead in the loop.
void runThroughputBenchmark(const std::vector<Order>& orders)
{
	OrderBook book;

	auto start = std::chrono::steady_clock::now();
	for(const auto& order : orders){
		book.processOrder(order);
	}
	auto end = std::chrono::steady_clock::now();

	double seconds = std::chrono::duration<double>(end - start).count();
	double ordersPerSec = orders.size() / seconds;

	std::cout << "Single-instrument throughput: " << orders.size() << " orders in "
		<< seconds * 1000 << " ms => " << static_cast<uint64_t>(ordersPerSec) << " orders/sec"
		<< std::endl;
}

} // namespace

int main()
{
	constexpr size_t kWarmupOrders = 10'000;
	constexpr size_t kLatencySamples = 100'000;
	constexpr size_t kThroughputOrders = 1'000'000;

	auto latencyOrders = generateOrders(kWarmupOrders + kLatencySamples, /*numSecurities=*/1, /*seed=*/42);
	runLatencyBenchmark(latencyOrders, kWarmupOrders);

	auto throughputOrders = generateOrders(kThroughputOrders, /*numSecurities=*/1, /*seed=*/1337);
	runThroughputBenchmark(throughputOrders);

	return 0;
}
