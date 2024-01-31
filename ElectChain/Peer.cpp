#include "Peer.h"



Peer::Peer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
    : _io_context(io_context), _acceptor(io_context, endpoint), _port(endpoint.port())
{
    _blockRequestHandler = std::make_shared<BlockRequestHandler>(this);
}

void Peer::startAccept()
{
    auto socket = std::make_shared<tcp::socket>(_io_context);

    _acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Accepted connection from: " << socket->remote_endpoint() << std::endl;
            _sockets.push_back(socket);

            createConnectionSocket(socket);
        }

    // Continue accepting
    startAccept();
        });
}

void Peer::createConnectionSocket(std::shared_ptr<tcp::socket> socket)
{
    auto sharedSocket = std::make_shared<tcp::socket>(std::move(*socket));
    std::thread(&Peer::startRead, this, sharedSocket, sharedSocket->remote_endpoint()).detach();
}

void Peer::startRead(std::shared_ptr<tcp::socket> socket, const tcp::endpoint& endpoint) {
    while (true) {
        auto buffer = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(*socket, *buffer, '\n', [this, socket, buffer, endpoint](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (!ec)
            {
                std::string msg = getMessage(buffer);
                std::cout << "Received message from " << endpoint << ": " << msg << std::endl;

                // Handle request
                _blockRequestHandler->handleRequest(msgToReqInfo(msg));

            }
            else {
                std::cerr << "Error reading from: " << endpoint << ": " << ec.message() << std::endl;
            }
            });
    }
}


RequestInfo Peer::msgToReqInfo(std::string msg)
{
    RequestInfo reqInfo;
    reqInfo.id = msg.substr(0, CODE_SIZE);
    reqInfo.buffer = strToVec(msg);

    return reqInfo;
}

std::string Peer::getMessage(std::shared_ptr<boost::asio::streambuf> buffer)
{
    std::istream is(buffer.get());
    std::string message;
    std::getline(is, message);

    return message;
}

void Peer::connect(const tcp::endpoint& endpoint) {
    auto socket = std::make_shared<tcp::socket>(_io_context);

    socket->async_connect(endpoint, [this, socket, endpoint](const boost::system::error_code& ec) {
        if (!ec)
        {
            _sockets.push_back(socket);

            std::cout << "Connected to: " << endpoint << std::endl;
        }
        else
        {
            std::cerr << "Error connecting to peer: " << ec.message() << std::endl;
        }
        });
}

void Peer::sendMsg(std::shared_ptr<tcp::socket> socket)
{
    std::string message = getMsg();

    auto buffer = convertMsgIntoBuffer(message);

    sendMsgToSocket(socket, buffer);
}

std::string Peer::getMsg()
{
    std::string message = "";
    std::cout << "Enter message to send: ";
    std::getline(std::cin, message);

    return message;
}

void Peer::sendBlock(std::shared_ptr<tcp::socket> socket, const Block& block)
{
    std::string serializedBlock = Serializer::serializeMessageBlock(block);
    sendMsgToSocket(socket, convertMsgIntoBuffer(serializedBlock));
}

Block Peer::receiveBlock(std::shared_ptr<tcp::socket> socket)
{
    std::string receivedMsg = getMessage(socket);
    std::vector<unsigned char> byteVector(receivedMsg.begin(), receivedMsg.end());
    return Deserializer::deserializeMessageBlock(byteVector);
}

std::shared_ptr<boost::asio::streambuf> Peer::convertMsgIntoBuffer(std::string msg)
{
    auto buffer = std::make_shared<boost::asio::streambuf>();
    std::ostream os(buffer.get());
    os << msg << '\n';

    return buffer;
}
std::string Peer::getMessage(std::shared_ptr<tcp::socket> socket)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(*socket, buffer, '\n');
    std::istream is(&buffer);
    std::string message;
    std::getline(is, message);
    return message;
}

void Peer::sendMsgToSocket(std::shared_ptr<tcp::socket> socket, std::shared_ptr<boost::asio::streambuf> buffer)
{
    boost::asio::async_write(*socket, *buffer, [this, socket, buffer](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
        if (!ec) {
            std::cout << "Message sent successfully." << std::endl;
        }
        else {
            std::cerr << "Error writing to peer: " << ec.message() << std::endl;
        }
        });
}

void Peer::findPeer(const PeerStruct& peer)
{
    std::cout << "Connecting to peer at: " << peer.peerEndpoint << std::endl;
    connect(peer.peerEndpoint);
}

std::shared_ptr<tcp::socket> Peer::getSocketByEndpoints(PeerStruct peer)
{
    for (auto socket : this->_sockets)
    {
        if (socket->remote_endpoint() == peer.peerEndpoint)
        {
            return socket;
        }
    }

    return nullptr;
}

void Peer::sendBroadcastMsg(std::string msg)
{
    auto bufferMsg = convertMsgIntoBuffer(msg);
    for (auto socket : this->_sockets)
    {
        if (socket->is_open())
        {
            sendMsgToSocket(socket, bufferMsg);
        }
    }
}

void Peer::sharePublicKey() {
    if (BlockchainUtils::pKeys) 
    {
        std::string publicKeyString = BlockchainUtils::publicKeyToString(BlockchainUtils::pKeys->publicKey);

        // Broadcast the public key to other peers
        sendBroadcastMsg(publicKeyString);
    }
}

void Peer::closePeer()
{
    closeOpenSockets();

    boost::system::error_code ec;
    _acceptor.close(ec);
    _io_context.stop();
}

void Peer::closeOpenSockets()
{
    for (auto& socket : _sockets)
    {
        if (socket->is_open())
        {
            boost::system::error_code ec;
            socket->shutdown(tcp::socket::shutdown_both, ec);
            socket->close(ec);
        }
    }
}