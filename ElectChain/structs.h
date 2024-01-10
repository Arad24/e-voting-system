#pragma once
#include <vector>
#include <iostream>
#include <string>
# include <openssl/sha.h>
# include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "RequestHandler.h"

struct ShareKeyRequest {
    int userId;
    std::vector<unsigned char> publicKey;
    std::time_t timestamp;

    ShareKeyRequest(int id, std::vector<unsigned char> key, std::time_t time) : userId(id), publicKey(key), timestamp(time) {}
};

struct RequestResult {
    std::string response;
    RequestHandler* newHandler;
};

struct ErrorResponse
{
    std::string message;
};
