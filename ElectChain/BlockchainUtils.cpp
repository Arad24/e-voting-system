# include "BlockchainUtils.h"

//std::string BlockchainUtils::calculateHash(const std::string& data)
//{
//    // Initialize SHA256 context
//    SHA256_CTX sha256;
//    SHA256_Init(&sha256);
//
//    // Update the context with the data
//    SHA256_Update(&sha256, data.c_str(), data.length());
//
//    // Finalize the hash
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    SHA256_Final(hash, &sha256);
//
//    // Convert the hash to a string
//    std::string hashStr;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
//        char buf[3];
//        sprintf_s(buf, "%02x", hash[i]);
//        hashStr += buf;
//    }
//
//    return hashStr;
//}


std::string BlockchainUtils::calculateHash(const std::string& data)
{
    return std::string();
}

bool BlockchainUtils::isValidHash(std::string blockHash)
{
    return (blockHash.substr(0, 2) == VALID_STARTWITH_HASH);
}
