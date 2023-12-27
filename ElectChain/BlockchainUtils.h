#pragma once
#pragma warning(disable : 4996)
# include <iostream>
# include <openssl/sha.h>
# include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <vector>

# define VALID_STARTWITH_HASH "00"
# define BITS 2048

struct KeyPair
{
	RSA* privateKey;
	RSA* publicKey;
};

// Functions
void freeAllRsa(BIO* bpPublic, BIO* bpPrivate, RSA* r, BIGNUM* bne);
bool saveKeys(BIO* bp_public, BIO* bp_private, RSA* r);
bool generateRsaKeys(RSA* r, BIGNUM* bne, unsigned long	e);
bool handleGenerateKeys(std::shared_ptr<KeyPair> pairKeys);



class BlockchainUtils
{
	public:
		static std::shared_ptr<KeyPair> pKeys;
		static std::string calculateHash(const std::string& data);
		static bool isValidHash(std::string blockHash);
		static std::shared_ptr<KeyPair> generateKeys();
		static RSA* vectorToRSA(const std::vector<unsigned char>& keyBytes);
		
};
