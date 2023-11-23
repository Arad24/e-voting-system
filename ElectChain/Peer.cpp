#include "Peer.h"

Peer::Peer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
    : _io_context(io_context), _acceptor(io_context, endpoint), _port(endpoint.port()) 
{
}

void Peer::startAccept() 
{
    while (true) 
    {
        auto socket = std::make_shared<tcp::socket>(_io_context);

        _acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
            if (!ec) {
                std::cout << "Accepted connection from: " << socket->remote_endpoint() << std::endl;

                std::thread(&Peer::startRead, this, socket, socket->remote_endpoint()).detach();
            }
            });
    }
}

void Peer::startRead(std::shared_ptr<tcp::socket> socket, const tcp::endpoint& endpoint) {
    while (true) {
        auto buffer = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(*socket, *buffer, '\n', [this, socket, buffer, endpoint](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);
                std::cout << "Received message from " << endpoint << ": " << message << std::endl;

            }
            else {
                std::cerr << "Error reading from: " << endpoint << ": " << ec.message() << std::endl;
            }
            });
    }
}

void Peer::connect(const tcp::endpoint& endpoint) {
    auto socket = std::make_shared<tcp::socket>(_io_context);

    socket->async_connect(endpoint, [this, socket, endpoint](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Connected to: " << endpoint << std::endl;

            this->startWrite(socket);
        }
        else {
            std::cerr << "Error connecting to peer: " << ec.message() << std::endl;
        }
        });
}

void Peer::startWrite(std::shared_ptr<tcp::socket> socket) 
{
    std::string message;
    std::cout << "Enter message to send: ";
    std::getline(std::cin, message);

    auto buffer = std::make_shared<boost::asio::streambuf>();
    std::ostream os(buffer.get());
    os << message << '\n';

    boost::asio::async_write(*socket, *buffer, [this, socket, buffer](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
        if (!ec) {
            std::cout << "Message sent successfully." << std::endl;
            this->startWrite(socket);
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
