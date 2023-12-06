#pragma
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Peer : public std::enable_shared_from_this<Peer> {
public:
    Peer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
        : io_context_(io_context), acceptor_(io_context, endpoint) {
        startAccept();
    }

    void findPeers(const std::vector<tcp::endpoint>& peerEndpoints) {
        for (const auto& endpoint : peerEndpoints) {
            connect(endpoint);
        }
    }

private:
    void startAccept() {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
            if (!ec) {
                std::cout << "Accepted connection from: " << socket->remote_endpoint() << std::endl;
                startRead(socket);
            }
        startAccept();
            });
    }

    void startRead(std::shared_ptr<tcp::socket> socket) {
        auto buffer = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(*socket, *buffer, '\n', [this, socket, buffer](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);
                std::cout << "Received message: " << message << " from: " << socket->remote_endpoint() << std::endl;
                startRead(socket);  // Continue reading
            }
            else {
                std::cerr << "Error reading from: " << socket->remote_endpoint() << ": " << ec.message() << std::endl;
            }
            });
    }

    void connect(const tcp::endpoint& endpoint) {
        auto socket = std::make_shared<tcp::socket>(io_context_);

        // Capture 'socket' and 'this' in the lambda's capture list
        auto self = shared_from_this();

        socket->async_connect(endpoint, [self, socket](const boost::system::error_code& ec) {
            if (!ec) {
                std::cout << "Connected to: " << socket->remote_endpoint() << std::endl;

                // Use 'self' to call startWrite
                self->startWrite(socket);
            }
            else {
                std::cerr << "Error connecting to peer: " << ec.message() << std::endl;
            }
            });
    }

    void startWrite(std::shared_ptr<tcp::socket> socket) {
        std::string message;
        std::cout << "Enter message to send: ";
        std::getline(std::cin, message);

        auto buffer = std::make_shared<boost::asio::streambuf>();
        std::ostream os(buffer.get());
        os << message << '\n';

        // Capture 'socket' and 'this' in the lambda's capture list
        auto self = shared_from_this();

        boost::asio::async_write(*socket, *buffer, [self, socket, buffer](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                std::cout << "Message sent successfully." << std::endl;

                // Use 'self' to call startWrite
                self->startWrite(socket);
            }
            else {
                std::cerr << "Error writing to peer: " << ec.message() << std::endl;
            }
            });
    }


private:
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};