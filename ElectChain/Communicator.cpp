# include "Communicator.h"


Communicator::Communicator(std::string host, std::string port, std::shared_ptr<net::io_context> new_ioc)
{
    ioc = new_ioc;
    createWsConnection(host, port);
}

void Communicator::createWsConnection(std::string host, std::string port)
{
    try
    {
        tcp::resolver resolver(*ioc);
        this->ws = std::make_shared<websocket::stream<tcp::socket>>(*ioc);

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws->next_layer(), results.begin(), results.end());

        // Perform the websocket handshake
        ws->handshake("WEB_IP", "/");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}


std::string Communicator::loginRequest(std::string username, std::string password, std::string peerAddress)
{
    std::string retMsg = "";
    std::string message = std::string(LOGIN_CODE) + "{'username':'" + username + "','password':'" + password + "','peer_address':'" + peerAddress + "'}";

    try
    {
        if (ws)
        {
            ws->write(net::buffer(message));
            retMsg = readMsg();
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return retMsg;
}


std::string Communicator::readMsg()
{
    beast::flat_buffer buffer;
    if (ws)
    {
        ws->read(buffer);
        std::cout << "Received: " << beast::make_printable(buffer.data()) << std::endl;
    }

    return boost::beast::buffers_to_string(buffer.data());
}



void Communicator::startHandleRequests()
{
    try
    {
        while (true)
        {
            std::string webMsg = readMsg();

            /*
                TODO: Call the handler and send the response to the server
            */
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}