# include "Peer.h"
# include "Communicator.h"
#include "Blockchain.h"
# include "StringUtils.h"
# include <iostream>

# define LOCAL_IP "localhost"
# define PORT "8881"

bool Login(std::shared_ptr<Communicator> cm, std::string peer_address);

static std::string g_userUid = "";

int main()
{
    bool login = false;
    std::shared_ptr<net::io_context> ioc_web = std::make_shared<net::io_context>();
    std::shared_ptr<Communicator> cm;

    try
    {
        cm = std::make_shared<Communicator>("localhost", PORT, ioc_web);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1;
    }

    /*
        TODO: Create peer - load ip and port from json file if exist
    */
    
    /*
        TODO: create peer
        Save in json file
        peer ip,
        peer port,

        save in {uid}.pem
        private key
        public key
    */
    
    while (!login)
    {
        login = Login(cm, "123:123");
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
        /*nlohmann::json jsonData = StringUtils::strToJson(res.substr(3, res.length()));
        if (jsonData.find("uid") != jsonData.end())
        {
            g_userUid = jsonData['uid'];
            return true;
        }*/

        return true;
    }

    return true;
}