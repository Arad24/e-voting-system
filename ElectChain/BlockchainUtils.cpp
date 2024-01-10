# include "BlockchainUtils.h"

std::shared_ptr<KeyPair> BlockchainUtils::pKeys;


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
    return (blockHash.substr(0, 2) == VALID_STARTWITH_HASH);
}

std::shared_ptr<KeyPair> BlockchainUtils::generateKeys()
{
	if (pKeys == nullptr) pKeys = std::make_shared<KeyPair>();

	if (handleGenerateKeys(pKeys)) return pKeys;
	else throw std::exception("Error generating keys.");
}

std::string BlockchainUtils::publicKeyToString(RSA* publicKey)
{
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(bio, publicKey);

	char* publicKeyData;
	long publicKeySize = BIO_get_mem_data(bio, &publicKeyData);
	std::string publicKeyString(publicKeyData, publicKeySize);

	BIO_free(bio);

	return publicKeyString;
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
 

std::string BlockchainUtils::signMessage(const std::string message)
{
	if (!pKeys) throw std::runtime_error("Private key is not available.");
	else if (!pKeys->privateKey) throw std::runtime_error("Private key is not available.");

	RSA* privateKey = pKeys->privateKey;

	// Hash the message (SHA-256 for example)
	std::string hashMsg = calculateHash(message);

	// Sign the hash
	unsigned char signature[SIGNATURE_LEN];
	unsigned int signatureLength;
	int ret = RSA_sign(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), SHA256_DIGEST_LENGTH, signature, &signatureLength, privateKey);
	if (ret != 1) {
		throw std::runtime_error("Error signing message.");
	}

	return base64Encode(signature, SIGNATURE_LEN);
}

std::string base64Encode(const unsigned char* input, size_t length) 
{
	BIO* bio = BIO_new(BIO_s_mem());
	BIO* b64 = BIO_new(BIO_f_base64());
	BIO_push(b64, bio);

	BIO_write(b64, input, static_cast<int>(length));
	BIO_flush(b64);

	BUF_MEM* buffer;
	BIO_get_mem_ptr(b64, &buffer);

	std::string result(buffer->data, buffer->length);

	BIO_free_all(b64);

	return result;
}

std::vector<unsigned char> base64Decode(const std::string& input)
{
	BIO* bio = BIO_new(BIO_s_mem());
	BIO_write(bio, input.c_str(), static_cast<int>(input.length()));

	BIO* b64 = BIO_new(BIO_f_base64());
	BIO_push(b64, bio);

	std::vector<unsigned char> buffer(input.length());
	int decodedLength = BIO_read(b64, buffer.data(), static_cast<int>(buffer.size()));

	BIO_free_all(b64);

	if (decodedLength <= 0) {
		throw std::runtime_error("Error decoding Base64.");
	}

	buffer.resize(decodedLength);
	return buffer;
}

RSA* findPublicKey(std::string uid)
{
	return nullptr;
}

bool BlockchainUtils::verifySignature(const std::string& message, const std::string& signMsg)
{
	RSA* publicKey = findPublicKey("uid");

	if (!publicKey) {
		throw std::runtime_error("Public key is not available.");
	}

	std::string hashMsg = calculateHash(message);

	std::vector<unsigned char> decodedSignature = base64Decode(signMsg);

	int ret = RSA_verify(NID_sha256, reinterpret_cast<const unsigned char*>(hashMsg.c_str()), SHA256_DIGEST_LENGTH, decodedSignature.data(), decodedSignature.size(), publicKey);

	RSA_free(publicKey);  

	return (ret == 1);
}

std::map<std::string, int> BlockchainUtils::countVotes(Blockchain& blockchain)
{
	std::map<std::string, int> votes;
	auto blocks = blockchain.getBlocks();

	try
	{
		for (auto block : blocks)
		{
			nlohmann::json blockData = nlohmann::json::parse(block.getData());
			std::string candidate = blockData["candidate"];

			(votes.find(candidate) != votes.end()) ? (votes[candidate]++) : (votes[candidate] = 1);
		}
		
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing block data: " << e.what() << std::endl;
	}

	return votes;
}

RSA* vectorToRSA(const std::vector<unsigned char>& keyBytes) {
	// Convert vector to C-style array
	const unsigned char* keyData = keyBytes.data();
	int keySize = static_cast<int>(keyBytes.size());

	// Use OpenSSL to load the key
	BIO* keyBio = BIO_new_mem_buf(keyData, keySize);
	RSA* rsaKey = PEM_read_bio_RSAPrivateKey(keyBio, nullptr, nullptr, nullptr);

	// Cleanup
	BIO_free(keyBio);

	return rsaKey;
}
