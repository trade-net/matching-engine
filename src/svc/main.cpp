#include <matcher_request_processor.h>
#include <tcp_async_server.h>

int main(){
	MatcherRequestProcessor processor(1, 1);
	TcpAsyncServer server(processor);
	server.run();
	return 0;
}
