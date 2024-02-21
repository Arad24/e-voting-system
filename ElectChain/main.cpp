# include "Peer.h"
# include "Communicator.h"
#include "Blockchain.h"
# include <iostream>

# define LOCAL_IP "127.0.0.1"
# define PORT 8820

bool Login();

int main()
{
    bool login = false;
    std::shared_ptr<Communicator> cm;
    try
    {
        cm = std::make_shared<Communicator>("localhost", "8881");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
    
    
    while (!login)
    {
        login = Login();
    }
    
    return 0;
}

bool Login()
{
    std::string uName = "", password = "";

    std::cout << "Enter username: ";
    std::cin >> uName;
    std::cout << "Enter password: ";
    std::cin >> password;

    // TODO: Send message to website - LOGIN_CODE{“username":"username",“password":"password","peer_address":"address"}
    // If get success code return true, else return false
    return true;
}