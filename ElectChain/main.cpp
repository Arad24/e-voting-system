# include "Peer.h"
# include "Communicator.h"
# include "Blockchain.h"
# include "StringUtils.h"
# include <iostream>
# include <fstream>

# define LOCAL_IP "localhost"
# define WEB_PORT "8881"
# define BLOCKCHAIN_FILENAME "bcCopy.csv"

bool Login(std::shared_ptr<Communicator> cm, std::string peer_address);
bool loadKeys();

static std::string g_userUid = "";

int main()
{
    // Create a blockchain
    std::shared_ptr<Blockchain> blockchain = std::make_shared<Blockchain>();
    BlockchainUtils::_bcCopy = blockchain;
    blockchain->loadFromFile();


    bool login = false;
    std::shared_ptr<net::io_context> ioc = std::make_shared<net::io_context>();
    std::shared_ptr<Communicator> cm;
    std::shared_ptr<Peer> peer;

    try
    {
        peer = std::make_shared<Peer>(*ioc, blockchain);
        cm = std::make_shared<Communicator>("localhost", WEB_PORT, ioc, peer->getBlockRequetHandler());
        
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1;
    }

    while (!login)
    {
        login = Login(cm, peer->getPeerAddress());
    }

    if (!loadKeys())
    {
        BlockchainUtils::generateKeys();
    }
    
    if (blockchain->getBlocks().empty())
    {
        blockchain->addGenesisBlock();
    }
    

    std::thread p2pThread(&Peer::startAccept, peer.get());
    p2pThread.detach();
    std::thread serverThread(&Communicator::startHandleRequests, cm.get());
    serverThread.join();


    ioc->run();
    return 0;
}

bool Login(std::shared_ptr<Communicator> cm, std::string peer_address)
{
    std::string uName = "", password = "";
    std::string res = "";

    std::cout << "Enter username: ";
    std::cin >> uName;
    std::cout << "Enter password: ";
    std::cin >> password;

    res = cm->loginRequest(uName, password, peer_address);

    if (res.rfind(LOGIN_FAILED_CODE, 0) == 0)
    {
        return false;
    }
    else if (res.rfind(LOGIN_SUCCEEDED_CODE, 0) == 0)
    {
        std::cout << res.substr(3);
        auto jsonMsg = nlohmann::json::parse(res.substr(3));
        BlockchainUtils::_userUid = jsonMsg["uid"];
        g_userUid = BlockchainUtils::_userUid;

        return true;
    }

    return false;
}




bool loadKeys()
{
    if (g_userUid != "")
    {
        std::string fileName = g_userUid + ".pem";
        return BlockchainUtils::loadKeysFromFile(fileName);
    }
    return false;
}