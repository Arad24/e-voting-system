# include <iostream>
# include "Peer.h"
# include <thread>

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 8888);
        Peer peer(io_context, endpoint);

        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}