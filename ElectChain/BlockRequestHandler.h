#pragma once
#include "Peer.h"
# include "IRequestHandler.h"
#include <iostream>

class BlockRequestHandler : public IRequestHandler
{
    public:
        BlockRequestHandler(std::shared_ptr<Peer> peer, std::shared_ptr<Blockchain> blockchain);

        RequestResult handleRequest(const Message& request);
        bool isRequestRelevant(RequestInfo rInfo);

    private:
        RequestResult handleShareKey(const ShareKeyRequest& shareKeyRequest);
        RequestResult handleAddBlock(Block blockToAdd);
        void shareBlockInTheNetwork(Block block);

        std::shared_ptr<Peer> _peer;
        std::shared_ptr<Blockchain> _blockchain;
};