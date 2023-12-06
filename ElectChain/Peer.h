# pragma once
#include <iostream>
#include <string>
#include <vector>
# include <thread>
#include <boost/asio.hpp>
#include <mutex>
#include <openssl/sha.h>

using boost::asio::ip::tcp;

struct PeerStruct
{
    tcp::endpoint peerEndpoint;

    PeerStruct(std::string ip, int port)
        : peerEndpoint(boost::asio::ip::address::from_string(ip), port) {}
};

std::mutex _locker;

class Peer
{
    public:
        Peer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint);
        void startAccept();

        void startRead(std::shared_ptr<tcp::socket> socket, const tcp::endpoint& endpoint);

        void connect(const tcp::endpoint& endpoint);

        void sendMsg(std::shared_ptr<tcp::socket> socket);
        void findPeer(const PeerStruct& peerEndpoints);

        void sendBroadcastMsg(std::string msg);

    private:
        std::string getMessage(std::shared_ptr<boost::asio::streambuf> buffer);
        std::shared_ptr<tcp::socket> getSocketByEndpoints(PeerStruct peer);
        void sendMsgToSocket(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::asio::streambuf> buffer);
        std::shared_ptr<boost::asio::streambuf> convertMsgIntoBuffer(std::string msg);
        std::string getMsg();

        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;
        int _port;
        std::vector<std::shared_ptr<tcp::socket>> _sockets;
};
