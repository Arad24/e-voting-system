# pragma once
# include "StringUtils.h"
# include "Deserializer.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <string>
#include <future>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

# define WEB_IP "localhost"

class Communicator
{
	public:
		Communicator(std::string host, std::string port, std::shared_ptr<net::io_context> new_ioc, std::shared_ptr<BlockRequestHandler> brh);
		std::string loginRequest(std::string username, std::string password, std::string peerAddress);
		void startHandleRequests();

	private:
		std::shared_ptr<websocket::stream<tcp::socket>> ws;
		std::shared_ptr<net::io_context> ioc;
		std::shared_ptr<BlockRequestHandler> _blockRequestHandler;

		void createWsConnection(std::string host, std::string port);
		std::string readMsg();
		Message convertToStructMessage(std::string msg);
};
