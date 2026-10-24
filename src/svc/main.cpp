#include <matcher_request_processor.h>
#include <tcp_async_server.h>

#include <cstdlib>

// usage: matcher [numIoThreads]
int main(int argc, char** argv){
	size_t numIoThreads = argc > 1 ? std::strtoul(argv[1], nullptr, 10) : 1;

	MatcherRequestProcessor processor(4);
	network::TcpAsyncServer server(processor, numIoThreads);
	server.run();
	return 0;
}
