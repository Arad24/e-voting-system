#pragma once
#include <iostream>
#include <string>
#include <vector>
# include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct PeerStruct
{
    tcp::endpoint peerEndpoint;

    PeerStruct(std::string ip, int port)
        : peerEndpoint(boost::asio::ip::address::from_string(ip), port) {}
};

class Peer
{
    public:
        Peer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint);

        void findPeer(const PeerStruct& peerEndpoints);

    private:
        void startAccept();

        void startRead(std::shared_ptr<tcp::socket> socket);

        void connect(const tcp::endpoint& endpoint);

        void startWrite(std::shared_ptr<tcp::socket> socket);


        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;
        int _port;
};
