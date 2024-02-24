# include "Peer.h"
# include "Communicator.h"
#include "Blockchain.h"
# include "StringUtils.h"
# include <iostream>
#include <string>


int main() {
    std::string option;
    std::shared_ptr<Peer> peer;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Create Peer\n";
        std::cout << "2. Connect to Other Peer\n";
        std::cout << "3. Send Message to All Peers\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::getline(std::cin, option);

        if (option == "1") {
            std::string ip;
            int port;
            std::cout << "Enter your IP: ";
            std::getline(std::cin, ip);
            std::cout << "Enter your port: ";
            std::cin >> port;
            std::cin.ignore(); // Ignore the newline character
            boost::asio::io_context io_context;
            peer = std::make_shared<Peer>(io_context, tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
            peer->startAccept();
            io_context.run();
        }
        else if (option == "2") {
            if (!peer) {
                std::cout << "You need to create a peer first.\n";
                continue;
            }
            std::string ip;
            int port;
            std::cout << "Enter the IP you want to connect to: ";
            std::getline(std::cin, ip);
            std::cout << "Enter the port you want to connect to: ";
            std::cin >> port;
            std::cin.ignore(); // Ignore the newline character
            peer->connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
        }
        else if (option == "3") {
            if (!peer) {
                std::cout << "You need to create a peer first.\n";
                continue;
            }
            std::string message;
            std::cout << "Enter the message you want to send: ";
            std::getline(std::cin, message);
            peer->sendBroadcastMsg(message);
        }
        else if (option == "4") {
            if (peer) {
                peer->closePeer();
            }
            std::cout << "Exiting...\n";
            break;
        }
        else {
            std::cout << "Invalid option. Please choose again.\n";
        }
    }

    return 0;
}