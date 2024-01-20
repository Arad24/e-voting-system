#pragma once
#include "Peer.h"
# include "IRequestHandler.h"
#include <iostream>

class BlockRequestHandler : public IRequestHandler
{
    public:
        BlockRequestHandler(std::shared_ptr<Peer> peer);

        RequestResult handleRequest(const Message& request);
        bool isRequestRelevant(RequestInfo rInfo);

    private:
        RequestResult handleShareKey(const ShareKeyRequest& shareKeyRequest);
        RequestResult handleVoteBlock(const Message& voteBlockRequest);

        std::shared_ptr<Peer> _peer;
};