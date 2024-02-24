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

# define VALID_STARTWITH_HASH "000"
# define KEY_BITS 2048
# define POW_LENGTH 3
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
bool saveKeys(RSA* public_key, RSA* private_key);
bool generateRsaKeys(RSA** public_key, RSA** private_key);
bool handleGenerateKeys(std::shared_ptr<KeyPair> pairKeys);
std::vector<Block> getUserBlocks(Blockchain bc, std::string uid);


class BlockchainUtils
{
	
	public:
		static std::shared_ptr<KeyPair> _pKeys;
		static std::shared_ptr<Blockchain> _bcCopy;
		static std::string _userUid;

		static std::string getSurveyUidFromBlock(Block block);
		static std::string getUserUidFromBlock(Block block);
		static nlohmann::json dataToJson(std::string data);

		// Hash
		static std::string calculateHash(const std::string& data);
		static bool isValidHash(std::string blockHash);

		// Block validation
		static bool isAlreadyVote(Blockchain bc, std::string user_uid, std::string survey_uid);
		static bool isAlreadySharePK(Blockchain bc, std::string uid);
		static bool isVoteBlock(Block block);
		static bool isShareKeyBlock(Block block);
		static bool isValidVoteBlock(Block block);
		static bool isValidShareKeyBlock(Block block);


		// Signature
		static std::string signMessage(const std::string message, const RSA* privateKey);
		static bool verifySignature(const std::string& message, const std::string& signMsg, std::string uid);

		// Keys
		static std::shared_ptr<KeyPair> generateKeys();
		static std::string publicKeyToString(RSA* publicKey);
		static RSA* strToPK(std::string pk);
		static bool loadKeysFromFile(std::string fileName);

		// Votes
		static std::map<std::string, int> countVotes(Blockchain& blockchain, std::string survey_uid);
};
