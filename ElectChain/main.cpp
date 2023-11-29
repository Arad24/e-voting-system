# include "Peer.h"
# include <iostream>

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 8888);

        Peer peer(io_context, endpoint);

        std::thread serverThread([&] { peer.startAccept(); });
        serverThread.detach();

        int choice;
        std::string customMessage;

        do
        {
            std::cout << "\nMenu:\n";
            std::cout << "1. Connect to Peer\n";
            std::cout << "2. Send Custom Message\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                std::string peerIP;
                int peerPort;

                std::cout << "Enter Peer IP: ";
                std::cin >> peerIP;
                std::cout << "Enter Peer Port: ";
                std::cin >> peerPort;

                PeerStruct newPeer(peerIP, peerPort);
                peer.findPeer(newPeer);
                break;
            }
            case 2:
            {
                std::cout << "Enter custom message: ";
                std::cin.ignore(); // Ignore any newline characters in the input buffer
                std::getline(std::cin, customMessage);

                peer.sendBroadcastMsg(customMessage);
                break;
            }
            case 3:
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 3);

        io_context.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}