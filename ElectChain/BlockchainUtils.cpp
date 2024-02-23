# include "BlockchainUtils.h"

std::shared_ptr<KeyPair> BlockchainUtils::_pKeys;
std::shared_ptr<Blockchain> BlockchainUtils::_bcCopy;
std::string BlockchainUtils::_userUid;

std::string BlockchainUtils::calculateHash(const std::string& data)
{
	// Initialize SHA256 context
	SHA256_CTX sha256;
	SHA256_Init(&sha256);

	// Update the context with the data
	SHA256_Update(&sha256, data.c_str(), data.length());

	// Finalize the hash
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_Final(hash, &sha256);

	// Convert the hash to a string
	std::string hashStr;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
		char buf[3];
		sprintf_s(buf, "%02x", hash[i]);
		hashStr += buf;
	}

	return hashStr;
}

bool BlockchainUtils::isValidHash(std::string blockHash)
{
	return (blockHash.substr(0, POW_LENGTH) == VALID_STARTWITH_HASH);
}

std::shared_ptr<KeyPair> BlockchainUtils::generateKeys()
{
	if (_pKeys == nullptr) _pKeys = std::make_shared<KeyPair>();

	if (handleGenerateKeys(_pKeys)) return _pKeys;
	else throw std::exception("Error generating keys.");
}

std::string BlockchainUtils::publicKeyToString(RSA* publicKey)
{
	BIO* bio = BIO_new(BIO_s_mem());
	if (!bio) throw std::runtime_error("Failed to create memory BIO.");

	// Write the RSA public key to the BIO in PEM format
	if (!PEM_write_bio_RSA_PUBKEY(bio, publicKey))
	{
		BIO_free(bio);
		throw std::runtime_error("Failed to write RSA public key to memory BIO.");
	}

	// Get the data from the BIO
	char* pkStr;
	long keySize = BIO_get_mem_data(bio, &pkStr);
	if (keySize <= 0)
	{
		BIO_free(bio);
		throw std::runtime_error("Failed to get public key data from memory BIO.");
	}

	std::string pemString(pkStr, keySize);
	BIO_free(bio);

	return pemString;
}

RSA* BlockchainUtils::strToPK(std::string pk)
{
	BIO* bio = BIO_new_mem_buf(pk.c_str(), pk.length());
	if (!bio) {
		throw std::runtime_error("Failed to create memory BIO.");
	}

	RSA* rsaKey = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
	if (!rsaKey)
	{
		BIO_free(bio);
		throw std::runtime_error("Failed to read RSA public key from memory BIO.");
	}

	BIO_free(bio);

	return rsaKey;
}

bool handleGenerateKeys(std::shared_ptr<KeyPair> pairKeys)
{
	bool	ret = true;
	RSA* r = NULL;
	BIGNUM* bne = NULL;
	BIO* bpPublic = NULL, * bpPrivate = NULL;
	unsigned long	e = RSA_F4;

	ret = (generateRsaKeys(&r, &bne, e));
	if (!ret) goto free_all;

	ret = (saveKeys(&bpPublic, &bpPrivate, r));
	if (!ret) goto free_all;

	pairKeys->privateKey = PEM_read_bio_RSAPrivateKey(bpPrivate, NULL, NULL, NULL);
	pairKeys->publicKey = PEM_read_bio_RSA_PUBKEY(bpPublic, NULL, NULL, NULL);

free_all:
	freeAllRsa(bpPublic, bpPrivate, r, bne);

	return ret;
}

bool generateRsaKeys(RSA** r, BIGNUM** bne, unsigned long	e)
{
	int ret = 0;
	*bne = BN_new();
	ret = BN_set_word(*bne, e);
	if (ret != 1) return false;

	*r = RSA_new();
	ret = RSA_generate_key_ex(*r, KEY_BITS, *bne, NULL);
	if (ret != 1) return false;

	return true;
}

bool saveKeys(BIO** bp_public, BIO** bp_private, RSA* r)
{
	int ret = 0;

	// Save public key
	*bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(*bp_public, r);
	if (ret != 1) return false;

	// Save private key
	*bp_private = BIO_new_file("private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(*bp_private, r, NULL, NULL, 0, NULL, NULL);
	if (ret != 1) return false;

	// Rewind the BIOs before reading
	BIO_reset(*bp_public);
	BIO_reset(*bp_private);

	return true;
}

void freeAllRsa(BIO* bpPublic, BIO* bpPrivate, RSA* r, BIGNUM* bne)
{
	BIO_free_all(bpPublic);
	BIO_free_all(bpPrivate);
	RSA_free(r);
	BN_free(bne);
}


std::string BlockchainUtils::signMessage(const std::string message, const RSA* privateKey)
{
	if (!_pKeys) throw std::runtime_error("Private key is not available.");
	else if (!_pKeys->privateKey) throw std::runtime_error("Private key is not available.");

	// Hash the message (SHA-256 for example)
	std::string hashMsg = calculateHash(message);

	// Sign the hash
	unsigned char* signature = new unsigned char[RSA_size(privateKey)];
	unsigned int sigLen;
	if (!RSA_sign(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), hashMsg.length(),
		signature, &sigLen, const_cast<RSA*>(privateKey))) {
		// Error occurred while signing
		delete[] signature;
		throw std::runtime_error("Failed to sign the message.");
	}

	// Convert the signature to a string
	std::string signatureStr(reinterpret_cast<char*>(signature), sigLen);
	delete[] signature; // Free the memory allocated for the signature buffer

	return signatureStr;

}

RSA* findPublicKey(std::string uid, Blockchain bc)
{
	RSA* pk = nullptr;

	for (auto block : bc.getBlocks())
	{
		if (BlockchainUtils::isShareKeyBlock(block))
		{
			nlohmann::json jsonData = nlohmann::json::parse(block.getData());
			if (jsonData.find("public_key") != jsonData.end())
			{
				return BlockchainUtils::strToPK(jsonData["public_key"]);
			}
		}
	}

	return pk;
}

bool BlockchainUtils::verifySignature(const std::string& message, const std::string& signMsg, std::string uid)
{
	RSA* publicKey = (uid == BlockchainUtils::_userUid) ? (BlockchainUtils::_pKeys->publicKey) : findPublicKey(uid, *_bcCopy);

	if (!publicKey) {
		throw std::runtime_error("Public key is not available.");
	}

	std::string hashMsg = calculateHash(message);

	int result = RSA_verify(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), hashMsg.length(),
		reinterpret_cast<const unsigned char*>(signMsg.c_str()), signMsg.length(), publicKey);

	// Free the public key
	RSA_free(publicKey);

	return result == 1;
}

std::map<std::string, int> BlockchainUtils::countVotes(Blockchain& blockchain, std::string survey_uid)
{
	std::map<std::string, int> votes;
	auto blocks = blockchain.getBlocks();

	try
	{
		for (auto block : blocks)
		{
			if (isVoteBlock(block))
			{
				nlohmann::json blockData = nlohmann::json::parse(block.getData());
				if (blockData["survey_uid"] == survey_uid)
				{
					std::string candidate = blockData["vote"];

					(votes.find(candidate) != votes.end()) ? (votes[candidate]++) : (votes[candidate] = 1);
				}
			}
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing block data: " << e.what() << std::endl;
		throw e;
	}

	return votes;
}

std::vector<Block> getUserBlocks(Blockchain bc, std::string uid)
{
	std::vector<Block> userBlocks;
	for (auto block : bc.getBlocks())
	{
		nlohmann::json jsonData = nlohmann::json::parse(block.getData());
		if (jsonData["uid"] == uid)
		{
			userBlocks.push_back(block);
		}
	}

	return userBlocks;
}


bool BlockchainUtils::isAlreadyVote(Blockchain bc, std::string user_uid, std::string survey_uid)
{
	std::vector<Block> userBlocks = getUserBlocks(bc, user_uid);

	for (auto block : userBlocks)
	{
		nlohmann::json jsonData = nlohmann::json::parse(block.getData());
		if (jsonData.find("survey_uid") != jsonData.end())
		{
			if (jsonData["survey_uid"] == survey_uid) return true;
		}
	}

	return false;
}

bool BlockchainUtils::isAlreadySharePK(Blockchain bc, std::string uid)
{
	std::vector<Block> userBlocks = getUserBlocks(bc, uid);

	for (auto block : userBlocks)
	{
		nlohmann::json jsonData = nlohmann::json::parse(block.getData());
		if (jsonData.find("public_key") != jsonData.end())
		{
			return true;
		}
	}

	return false;
}

std::string BlockchainUtils::getUserUidFromBlock(Block block)
{
	std::string uid = "";
	nlohmann::json jsonData = nlohmann::json::parse(block.getData());
	if (jsonData.find("user_uid") != jsonData.end())
	{
		uid = jsonData["user_uid"];
	}

	return uid;
}

std::string BlockchainUtils::getSurveyUidFromBlock(Block block)
{
	std::string uid = "";
	nlohmann::json jsonData = nlohmann::json::parse(block.getData());
	if (jsonData.find("survey_uid") != jsonData.end())
	{
		uid = jsonData["survey_uid"];
	}

	return uid;
}

nlohmann::json BlockchainUtils::dataToJson(std::string data)
{

	nlohmann::json jsonData = nlohmann::json::parse(data);

	return jsonData;
}

bool BlockchainUtils::isVoteBlock(Block block)
{
	return (block.getData().find("vote") != 0);
}

bool BlockchainUtils::isShareKeyBlock(Block block)
{
	return (block.getData().find("public_key") != 0);
}

bool BlockchainUtils::isValidVoteBlock(Block block)
{
	nlohmann::json jsonData = BlockchainUtils::dataToJson(block.getData());
	std::string userUid = "", surveyUid = "", vote = "", signVote = "";
	if (jsonData.find("survey_uid") != jsonData.end()) surveyUid = jsonData["survey_uid"];
	if (jsonData.find("user_uid") != jsonData.end()) userUid = jsonData["user_uid"];
	if (jsonData.find("vote") != jsonData.end()) vote = jsonData["vote"];
	if (jsonData.find("sign_vote") != jsonData.end()) signVote = jsonData["sign_vote"];

	if (userUid == "" || surveyUid == "" || vote == "" || signVote == "") return false;

	return (_bcCopy->validateBlock(block) &&
		!BlockchainUtils::isAlreadyVote(*_bcCopy, userUid, surveyUid) &&
		BlockchainUtils::verifySignature(vote, signVote, userUid));
}

bool BlockchainUtils::isValidShareKeyBlock(Block block)
{
	return (_bcCopy->validateBlock(block)
		&& !BlockchainUtils::isAlreadySharePK(*_bcCopy, BlockchainUtils::getUserUidFromBlock(block)));
}