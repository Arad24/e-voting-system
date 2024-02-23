# include "Peer.h"
# include "Communicator.h"
# include "Blockchain.h"
# include "StringUtils.h"
# include <iostream>
# include <fstream>

# define LOCAL_IP "localhost"
# define WEB_PORT "8881"
# define PEER_DATA_FILE "info.json"

bool Login(std::shared_ptr<Communicator> cm, std::string peer_address);
bool loadKeys();

static std::string g_userUid = "";

int main()
{
    bool login = false;
    std::shared_ptr<net::io_context> ioc_web = std::make_shared<net::io_context>();
    std::shared_ptr<Communicator> cm;

    try
    {
        cm = std::make_shared<Communicator>("localhost", WEB_PORT, ioc_web);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1;
    }

    /*
        TODO: Create peer
    */
    
    while (!login)
    {
        login = Login(cm, "123:123");
    }

    if (!loadKeys())
    {
        BlockchainUtils::generateKeys();
    }
    
    
    ioc_web->run();
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
        g_userUid = BlockchainUtils::_userUid;

        return true;
    }

    return false;
}


bool loadKeys()
{
    std::string fileName = g_userUid + ".pem";
    return BlockchainUtils::loadKeysFromFile(fileName);
}