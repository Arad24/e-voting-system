#pragma once
# include "BlockchainUtils.h"
#include <iostream>
#include <chrono>
# include <string>

# define DEFAULT_HASH "0"

class BlockchainUtils;

class Block
{
    public:
        // Ctor
        Block(std::string prevHash, std::string hash, std::string data, std::string timestamp, int nonce);
        Block();
        Block(std::string data);

        void createBlock();

        // Getters
        std::string getData();
        std::string getHash();
        std::string getPrevHash();
        std::string getTimeStamp();

        // Setters
        void setPrevHash(std::string prevHash);
        void setData(std::string data);

        // Hash
        void mineHash();

    private:
        std::string _prevHash;
        std::string _hash;
        std::string _data;
        std::string _timestamp;
        int _nonce;

        std::string blockToStr();

        std::string getCurrentTimestamp();
};