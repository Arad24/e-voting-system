#pragma once
# include <iostream>
#include <chrono>
# include "BlockchainUtils.h"

# define DEFAULT_HASH "0"

class Block
{
	public:
		// Ctor
		Block(std::string prevHash, std::string hash, std::string data, std::string timestamp);
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


	private:
		std::string _prevHash;
		std::string _hash;
		std::string _data;
		std::string _timestamp;
};