#pragma once
# include <iostream>
#include <openssl/sha.h>
#include <openssl/evp.h>

class BlockchainUtils
{
public:
	static std::string calculateHash(const std::string& data);
};