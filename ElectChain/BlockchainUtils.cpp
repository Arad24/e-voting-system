# include "BlockchainUtils.h"

std::shared_ptr<KeyPair> BlockchainUtils::_pKeys;
std::shared_ptr<Blockchain> BlockchainUtils::_bcCopy = std::make_shared<Blockchain>();
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
	char* buffer = nullptr;

	PEM_write_bio_RSA_PUBKEY(bio, publicKey);

	size_t length = BIO_get_mem_data(bio, &buffer);

	std::string publicKeyString(buffer, length);
	std::string base64Key = StringUtils::base64_encode(publicKeyString);

	BIO_free(bio);
	
	return base64Key;
}

RSA* BlockchainUtils::strToPK(std::string pk)
{
	std::string decodedKey = StringUtils::base64_decode(pk);

	// Create a memory BIO to hold the decoded key data
	BIO* bio = BIO_new_mem_buf(decodedKey.c_str(), decodedKey.length());
	if (!bio) 
	{
		throw std::runtime_error("Failed to create memory BIO.");
	}

	// Read the RSA public key from the memory BIO
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
	RSA* private_key = NULL;
	RSA* public_key = NULL;

	if (!generateRsaKeys(&public_key, &private_key))
		return false;

	if (!saveKeys(public_key, private_key)) {
		RSA_free(public_key);
		RSA_free(private_key);
		return false;
	}

	if (!private_key || !public_key) return false;

	pairKeys->privateKey = private_key;
	pairKeys->publicKey = public_key;

	return true;
}

bool generateRsaKeys(RSA** public_key, RSA** private_key)
{
	BIGNUM* bne = NULL;
	RSA* r = RSA_new();
	if (!r) return false;

	bne = BN_new();
	if (!bne) {
		RSA_free(r);
		return false;
	}

	if (!BN_set_word(bne, RSA_F4)) {
		BN_free(bne);
		RSA_free(r);
		return false;
	}

	if (!RSA_generate_key_ex(r, KEY_BITS, bne, NULL)) {
		BN_free(bne);
		RSA_free(r);
		return false;
	}

	*public_key = RSAPublicKey_dup(r);
	*private_key = RSAPrivateKey_dup(r);

	BN_free(bne);
	RSA_free(r);

	if (!*public_key || !*private_key)
		return false;

	return true;
}

bool saveKeys(RSA* public_key, RSA* private_key)
{
	if (!public_key || !private_key) return false;

	if (BlockchainUtils::_userUid.empty()) return false;

	std::string filename = BlockchainUtils::_userUid + ".pem";
	BIO* bp = BIO_new_file(filename.c_str(), "w+");
	
	if (!bp) return false;

	int ret = 1;
	ret &= PEM_write_bio_RSAPublicKey(bp, public_key);
	ret &= PEM_write_bio_RSAPrivateKey(bp, private_key, NULL, NULL, 0, NULL, NULL);

	BIO_free(bp);

	return ret == 1;
}

bool BlockchainUtils::loadKeysFromFile(std::string fileName)
{
	BlockchainUtils::_pKeys = std::make_shared<KeyPair>();

	BIO* bp = BIO_new_file(fileName.c_str(), "r");

	if (!bp) return false;

	BlockchainUtils::_pKeys->publicKey = PEM_read_bio_RSAPublicKey(bp, NULL, NULL, NULL);

	if (!BlockchainUtils::_pKeys->publicKey)
	{
		BIO_free(bp);
		return false;
	}

	BlockchainUtils::_pKeys->privateKey = PEM_read_bio_RSAPrivateKey(bp, NULL, NULL, NULL);
	if (!BlockchainUtils::_pKeys->privateKey)
	{
		RSA_free(BlockchainUtils::_pKeys->publicKey); // Free the previously allocated public key
		BIO_free(bp);
		return false;
	}

	BIO_free(bp);
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

	std::string hashMsg = calculateHash(message); 

	// Sign the hash
	unsigned char* signature = new unsigned char[RSA_size(privateKey)];
	unsigned int sigLen;
	if (!RSA_sign(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), hashMsg.length(),
		signature, &sigLen, const_cast<RSA*>(privateKey))) 
	{
		// Error occurred while signing
		delete[] signature;
		throw std::runtime_error("Failed to sign the message.");
	}

	// Convert the signature to a string
	std::string signatureStr(reinterpret_cast<char*>(signature), sigLen);
	delete[] signature;

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
			if ((jsonData.find("public_key") != jsonData.end() && jsonData.find("user_uid") != jsonData.end()) &&
				jsonData["user_uid"] == uid)
			{
				return BlockchainUtils::strToPK(jsonData["public_key"]);
			}
		}
	}

	return pk;
}

bool BlockchainUtils::verifySignature(const std::string& message, const std::string& signMsg, std::string uid) {
	RSA* publicKey = (uid == BlockchainUtils::_userUid) ? (BlockchainUtils::_pKeys->publicKey) : findPublicKey(uid, *_bcCopy);

	if (!publicKey) {
		throw std::runtime_error("Public key is not available.");
	}

	std::string hashMsg = calculateHash(message);

	std::string decodedSignMsg = StringUtils::base64_decode(signMsg);

	int result = RSA_verify(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), hashMsg.length(),
		reinterpret_cast<const unsigned char*>(decodedSignMsg.c_str()), decodedSignMsg.length(), publicKey);

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

	for (auto& block : bc.getBlocks()) 
	{
		nlohmann::json jsonData;
		try 
		{
			jsonData = nlohmann::json::parse(block.getData());
		}
		catch (const nlohmann::json::parse_error&) 
		{
			continue;
		}

		if ((jsonData.contains("user_uid")) && (BlockchainUtils::isVoteBlock(block) || BlockchainUtils::isShareKeyBlock(block))) 
		{
			if (jsonData["user_uid"] == uid) 
			{
				userBlocks.push_back(block);
			}
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

std::string BlockchainUtils::createSharedKeyData()
{
	nlohmann::json data = {};
	if (!_pKeys || !_pKeys->publicKey || _userUid.empty()) {
		return ""; 
	}

	data["public_key"] = BlockchainUtils::publicKeyToString(_pKeys->publicKey);
	data["user_uid"] = _userUid;
	return data.dump();
}