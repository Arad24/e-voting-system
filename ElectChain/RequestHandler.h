// RequestHandler.h
#pragma once
#include "Block.h"
#include <iostream>

class RequestHandler {
public:
    static std::string handleRequest(const Message& request);
    RequestHandler(Peer* peer);
private:
    static std::string handleAddBlock(const Block& block);
    static std::string handleShareKey(const ShareKeyRequest& shareKeyRequest);
    static std::string handleVoteBlock(const Message& voteBlockRequest);
    static std::string handleErrorResponse();

    static Peer* _peer;
};