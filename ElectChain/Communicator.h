# pragma once
# include "StringUtils.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <string>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Communicator
{
	public:
		Communicator(std::string host, std::string port);
		void startHandleRequests();

	private:
		std::shared_ptr<websocket::stream<tcp::socket>> ws;
		std::shared_ptr<net::io_context> ioc;

		void createWsConnection(std::string host, std::string port);
};