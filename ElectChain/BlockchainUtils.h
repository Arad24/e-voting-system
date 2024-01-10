#pragma once
#pragma warning(disable : 4996)
# include "StringUtils.h"
# include "json.hpp"
# include "Blockchain.h"
# include <iostream>
# include <openssl/sha.h>
# include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <vector>

# define VALID_STARTWITH_HASH "00"
# define KEY_BITS 2048
# define SIGNATURE_LEN (KEY_BITS / 8)

class Block;
class Blockchain;

struct KeyPair
{
	RSA* privateKey;
	RSA* publicKey;
};

// Functions
void freeAllRsa(BIO* bpPublic, BIO* bpPrivate, RSA* r, BIGNUM* bne);
bool saveKeys(BIO** bp_public, BIO** bp_private, RSA* r);
bool generateRsaKeys(RSA** r, BIGNUM** bne, unsigned long	e);
bool handleGenerateKeys(std::shared_ptr<KeyPair> pairKeys);
std::string base64Encode(const unsigned char* input, size_t length);
std::vector<unsigned char> base64Decode(const std::string& input);


class BlockchainUtils
{
	public:
		static std::shared_ptr<KeyPair> pKeys;

		static std::string calculateHash(const std::string& data);
		static bool isValidHash(std::string blockHash);
		static std::shared_ptr<KeyPair> generateKeys();
		static std::string signMessage(const std::string message);
		bool verifySignature(const std::string& message, const std::string& signMsg);
		std::map<std::string, int> countVotes(Blockchain& blockchain);
		static RSA* vectorToRSA(const std::vector<unsigned char>& keyBytes);
		static std::string publicKeyToString(RSA* publicKey);
};
