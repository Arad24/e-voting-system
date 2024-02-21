# include "Communicator.h"


Communicator::Communicator(std::string host, std::string port)
{
    createWsConnection(host, port);
}

void Communicator::createWsConnection(std::string host, std::string port)
{
    // These objects perform our I/O
    ioc = std::make_shared<net::io_context>();
    tcp::resolver resolver(*ioc);
    this->ws = std::make_shared<websocket::stream<tcp::socket>>(*ioc);

    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    net::connect(ws->next_layer(), results.begin(), results.end());

    // Perform the websocket handshake
    ws->handshake("localhost", "/");
}
void Communicator::startHandleRequests()
{
    try
    {
        // Send a message to the server
        std::string message = "Hello from C++";
        ws->write(net::buffer(message));

        // Read a message from the server asynchronously
        beast::flat_buffer buffer;
        ws->async_read(buffer, [&](beast::error_code ec, std::size_t bytes_transferred) {
            if (ec == websocket::error::closed) {
                std::cout << "Connection closed by server\n";
                return;
            }
            if (ec) {
                std::cerr << "Error reading message: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Received: " << beast::make_printable(buffer.data()) << std::endl;
            });

        // Run the IO context
        ioc->run();

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}