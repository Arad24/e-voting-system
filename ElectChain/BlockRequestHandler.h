#pragma once
#include "Peer.h"
# include "IRequestHandler.h"
#include <iostream>

# define INVALID_REQUEST_ERROR "Invalid Request"

class Peer;

class BlockRequestHandler : public IRequestHandler
{
    public:
        BlockRequestHandler(std::shared_ptr<Peer> peer, std::shared_ptr<Blockchain> blockchain);

        RequestResult handleRequest(const Message& request);
        bool isRequestRelevant(Message rInfo);

    private:
        RequestResult handleShareKey(Block blockToAdd);
        RequestResult handleAddBlock(Block blockToAdd);
        RequestResult handleGetBlockchain(std::vector<Block> blocksList);
        void shareBlockInTheNetwork(Block block);
        

        std::shared_ptr<Peer> _peer;
        std::shared_ptr<Blockchain> _blockchain;
};