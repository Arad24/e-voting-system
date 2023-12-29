# include "Peer.h"
#include "Blockchain.h"
# include <iostream>

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::endpoint endpoint(boost::asio::ip::make_address("10.57.200.12"), 8888);

        Peer peer(io_context, endpoint);

        std::thread serverThread([&] { peer.startAccept(); });
        serverThread.detach();

        int choice;
        std::string customMessage;

        // Create a simple blockchain
        Blockchain blockchain;

        do
        {
            std::cout << "\nMenu:\n";
            std::cout << "1. Connect to Peer\n";
            std::cout << "2. Send Custom Message\n";
            std::cout << "3. Create and Share Block\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            getchar();

            switch (choice)
            {
            case 1:
            {
                std::string peerIP;
                int peerPort;

                std::cout << "Enter Peer IP: ";
                std::cin >> peerIP;
                getchar();
                std::cout << "Enter Peer Port: ";
                std::cin >> peerPort;
                getchar();

                PeerStruct newPeer(peerIP, peerPort);
                peer.findPeer(newPeer);
                break;
            }
            case 2:
            {
                std::cout << "Enter custom message: ";
                std::cin.ignore();
                std::getline(std::cin, customMessage);
                getchar();
                peer.sendBroadcastMsg(customMessage);
                break;
            }
            case 3:
            {
                std::string blockData;
                std::cout << "Enter block data: ";
                std::cin.ignore();
                std::getline(std::cin, blockData);
                Block newBlock(blockData);
                auto maxTimestamp = std::chrono::system_clock::now();

                try
                {
                    blockchain.addBlock(newBlock);
                }
                catch (const std::exception&)
                {
                    std::cout << "Block is not valid. It won't be added to the blockchain.\n";
                }
                
                std::string serializedBlock = Serializer::serializeMessage(newBlock);
                peer.sendBroadcastMsg(serializedBlock);
                std::cout << "Block is valid and added to the blockchain.\n";

                break;
            }
            case 4:
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }
            
            io_context.run();

        } while (choice != 4);

        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}