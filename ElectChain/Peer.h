#pragma once
#include <iostream>
#include <string>
#include <vector>
# include <thread>
#include <boost/asio.hpp>
#include <mutex>
#include <atomic>
#include "Block.h"
# include "BlockRequestHandler.h"

#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

using boost::asio::ip::tcp;

struct PeerStruct
{
    boost::asio::ip::tcp::endpoint peerEndpoint;

    PeerStruct(std::string ip, int port)
        : peerEndpoint(boost::asio::ip::address::from_string(ip), port) {}

    std::string toString() const {
        std::string ip = peerEndpoint.address().to_string();
        int port = peerEndpoint.port();
        return ip + ":" + std::to_string(port);
    }
};

class BlockRequestHandler;
class IRequestHandler;
class Deserializer;
class Serializer;

class Peer
{
    public:
        Peer(boost::asio::io_context& io_context, std::shared_ptr<Blockchain> bc);
        void startAccept();

        std::shared_ptr<BlockRequestHandler> getBlockRequetHandler();
        std::string getPeerAddress();

        void startRead(std::shared_ptr<tcp::socket> socket, const tcp::endpoint& endpoint);

        void connect(const tcp::endpoint& endpoint);

        void findPeer(const PeerStruct& peerEndpoints);

        void sendBroadcastMsg(std::string msg);
        void createConnectionSocket(std::shared_ptr<tcp::socket> socket);
        std::shared_ptr<Blockchain> getBlockchain();

        void closePeer();

        void sendLastHash();
    private:
        std::string getMessage(std::shared_ptr<boost::asio::streambuf> buffer);
        std::string getMessage(std::shared_ptr<tcp::socket> socket);
        std::shared_ptr<tcp::socket> getSocketByEndpoints(PeerStruct peer);
        void sendMsgToSocket(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::asio::streambuf> buffer);
        std::shared_ptr<boost::asio::streambuf> convertMsgIntoBuffer(std::string msg);
        Message& convertToStructMessage(std::string msg);

        void closeOpenSockets();

        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<tcp::socket>> _sockets;
        std::shared_ptr<Blockchain> _blockchain;
        std::shared_ptr<BlockRequestHandler> _blockRequestHandler;
        int _port;
};
