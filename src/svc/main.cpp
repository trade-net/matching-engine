#include <matcher_request_processor.h>
#include <logger.h>
#include <tcp_async_server.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

namespace{

logging::Level parseLevel(const std::string& name)
{
	if(name == "debug") return logging::Level::Debug;
	if(name == "info") return logging::Level::Info;
	if(name == "warn") return logging::Level::Warn;
	if(name == "error") return logging::Level::Error;
	if(name == "off") return logging::Level::Off;
	std::cerr << "unknown log level '" << name << "', using info" << std::endl;
	return logging::Level::Info;
}

} // namespace

// usage: matcher [numIoThreads=1] [logLevel=info] [statsIntervalMs=500, 0 disables] [port=1234]
int main(int argc, char** argv){
	size_t numIoThreads = argc > 1 ? std::strtoul(argv[1], nullptr, 10) : 1;
	logging::setLevel(parseLevel(argc > 2 ? argv[2] : "info"));
	unsigned statsIntervalMs = argc > 3 ? std::strtoul(argv[3], nullptr, 10) : 500;
	unsigned short port = argc > 4 ? std::strtoul(argv[4], nullptr, 10) : 1234;

	MatcherRequestProcessor processor(4);
	network::TcpAsyncServer server(processor, numIoThreads, port);

	// prints the exchange state whenever orders were processed since the last print
	std::atomic<bool> stop{false};
	std::thread monitor;
	if(statsIntervalMs > 0)
	{
		monitor = std::thread([&]{
			uint64_t lastPrinted = 0;
			while(!stop)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(statsIntervalMs));
				uint64_t processed = processor.ordersProcessed();
				if(processed == lastPrinted) continue;
				lastPrinted = processed;
				std::cout << formatExchangeState(processor.snapshotAll(/*depth=*/3)) << std::flush;
			}
		});
	}

	server.run();

	stop = true;
	if(monitor.joinable()) monitor.join();
	return 0;
}
