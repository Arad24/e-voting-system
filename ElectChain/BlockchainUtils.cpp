# include "BlockchainUtils.h"

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
	pKeys = std::shared_ptr<KeyPair>();

	if (handleGenerateKeys(pKeys)) return pKeys;
	else throw std::exception("Error generating keys.");

}

bool handleGenerateKeys(std::shared_ptr<KeyPair> pairKeys)
{
	int	ret = 1;
	RSA* r = NULL;
	BIGNUM* bne = NULL;
	BIO* bpPublic = NULL, * bpPrivate = NULL;
	unsigned long	e = RSA_F4;

	ret = (generateRsaKeys(r, bne, e)) ? (1) : (0);
	if (ret == 0) goto free_all;

	ret = (saveKeys(bpPublic, bpPrivate, r)) ? (1) : (0);
	if (ret == 0) goto free_all;

	pairKeys->privateKey = PEM_read_bio_RSAPrivateKey(bpPrivate, NULL, NULL, NULL);
	pairKeys->publicKey = PEM_read_bio_RSA_PUBKEY(bpPublic, NULL, NULL, NULL);


	free_all:
		freeAllRsa(bpPublic, bpPrivate, r, bne);

	return (ret == 1);
}


bool generateRsaKeys(RSA* r, BIGNUM* bne, unsigned long	e)
{
	int ret = 0;
	bne = BN_new();
	ret = BN_set_word(bne, e);
	if (ret != 1) return false;

	r = RSA_new();
	ret = RSA_generate_key_ex(r, BITS, bne, NULL);
	if (ret != 1) return false;

	return true;
}

bool saveKeys(BIO* bp_public, BIO* bp_private, RSA* r)
{
	int ret = 0;

	// Save public key
	bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if (ret != 1) return false;

	// Save private key
	bp_private = BIO_new_file("private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);
	if (ret != 1) return false;

	return true;
}

void freeAllRsa(BIO* bpPublic, BIO* bpPrivate, RSA* r, BIGNUM* bne)
{
	
	BIO_free_all(bpPublic);
	BIO_free_all(bpPrivate);
	RSA_free(r);
	BN_free(bne);
}