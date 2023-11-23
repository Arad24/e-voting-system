# include <iostream>
# include "Peer.h"
# include <thread>

int main()
{
    try
    {
        boost::asio::io_context io_context;
        
        tcp::endpoint endpoint(boost::asio::ip::make_address("192.168.218.1"), 8888);

        Peer peer(io_context, endpoint);
        std::thread serverThread([&] { io_context.run(); });
        serverThread.detach();

        PeerStruct newPeer("192.168.1.163", 8888);
        peer.findPeer(newPeer);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}