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
        Block(std::string prevHash, std::string hash, std::string data, int index, int nonce);
        Block();
        Block(std::string data);

        void createBlock();

        // Getters
        std::string getData();
        std::string getHash();
        std::string getPrevHash();
        int getIndex();
        int getNonce();

        // Setters
        void setPrevHash(std::string prevHash);
        void setData(std::string data);
        void setHash(std::string hash);
        void setIndex(int index);
        void setNonce(int nonce);

        // Hash
        void mineHash();



    private:
        std::string _prevHash;
        std::string _hash;
        std::string _data;
        int _index;
        int _nonce;

        std::string blockToStr();
};