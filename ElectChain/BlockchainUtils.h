#pragma once
# include <iostream>
# include <openssl/sha.h>
# include <openssl/evp.h>

# define VALID_STARTWITH_HASH "00"


class BlockchainUtils
{
	public:
		static std::string calculateHash(const std::string& data);
		static bool isValidHash(std::string blockHash);
};