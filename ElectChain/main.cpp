# include <iostream>
# include "Peer.h"

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::endpoint endpoint(boost::asio::ip::make_address("10.100.102.72"), 8888);

        Peer peer(io_context, endpoint);

        std::thread serverThread([&] { peer.startAccept(); });
        serverThread.detach();

        
        PeerStruct newPeer("192.168.1.163", 8888);
        peer.findPeer(newPeer);

        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}