# pragma once
# include "StringUtils.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core.hpp>
# include "LoginRequestHandler.h"
#include <iostream>
#include <thread>
# include <map>
#include <mutex>


namespace beast = boost::beast;
namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>


# define LOCAL_IP "127.0.0.1"
# define PORT 8820

class Communicator
{
	public:
		Communicator(RequestHandlerFactory& factory);
		void startHandleRequests();
		void bindAndListen();

	private:
		// Private functions
		void addClientToMap(std::shared_ptr< websocket::stream<tcp::socket>> cSocket);

		// Server communicate
		void handleNewClient(std::shared_ptr< websocket::stream<tcp::socket>> clientSocket);
		void handleClient(std::shared_ptr<websocket::stream<tcp::socket>> clientSocket);
		void acceptClients(tcp::acceptor* acceptor, boost::asio::io_context* io_context);

		// Connect with client
		std::string getMsgFromClient(websocket::stream<tcp::socket>& ws);
		void sendMsgToClient(websocket::stream<tcp::socket>& ws, std::string msg);
		RequestInfo msgToReqInfo(std::string msg);


		RequestHandlerFactory& m_handlerFactory;
		std::map<std::shared_ptr<websocket::stream<tcp::socket>>, std::shared_ptr<IRequestHandler>> m_clients;

		std::mutex _mtx;
};