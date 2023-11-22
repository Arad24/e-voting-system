# include <iostream>
# include "Peer.h"
# include <thread>

int main()
{
    try
    {
        boost::asio::io_context io_context;

        // Create a Peer object and specify the endpoint to listen on
        tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 8888);
        Peer peer(io_context, endpoint);

        // You can use findPeer to connect to another peer if needed
        // For example:
        // PeerStruct peerInfo;
        // peerInfo.peerEndpoint = tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8889);
        // peer.findPeer(peerInfo);

        // Run the IO context to start handling asynchronous operations
        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}