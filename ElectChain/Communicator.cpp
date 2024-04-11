# include "Communicator.h"


Communicator::Communicator(std::string host, std::string port, std::shared_ptr<net::io_context> new_ioc, std::shared_ptr<BlockRequestHandler> brh)
{
    ioc = new_ioc;
    createWsConnection(host, port);
    _blockRequestHandler = brh;
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
    std::string message = std::string(LOGIN_CODE) + "{\"username\":\"" + username + "\",\"password\":\"" + password + "\",\"peer_address\":\"" + peerAddress + "\"}";

    try
    {
        if (ws)
        {
            ws->write(net::buffer(message));
            retMsg = readMsg();
            std::cout << retMsg;
            return retMsg;
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
        const std::string getPeersMsg = std::string(GET_PEERS_LIST) + "{}";
        ws->write(net::buffer(getPeersMsg));
        while (true)
        {
            std::string webMsg = readMsg();


            auto res = _blockRequestHandler->handleRequest(convertToStructMessage(webMsg));

            if (res.response[0] != DONT_SEND_CODE[0]) ws->write(net::buffer(res.response));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

Message Communicator::convertToStructMessage(std::string msg)
{
    Message structMsg(msg.substr(0, 3), StringUtils::strToVec(msg));
    return structMsg;
}
