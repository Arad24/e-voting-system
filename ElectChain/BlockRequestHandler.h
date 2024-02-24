#pragma once
#include "Peer.h"
# include "IRequestHandler.h"
#include <iostream>

# define INVALID_REQUEST_ERROR "Invalid Request"
# define ERROR_RESPONSE "Error accured in the server"
# define SUCCESS_RESPONSE "Success"

class Peer;
class Blockchain;
class Block;
struct Message;


class BlockRequestHandler : public IRequestHandler
{
    public:
        BlockRequestHandler(std::shared_ptr<Peer> peer, std::shared_ptr<Blockchain> blockchain);

        RequestResult handleRequest(Message& request) override;
        bool isRequestRelevant(Message& request) override;

    private:
        RequestResult handleShareKey(Block blockToAdd);
        RequestResult handleAddBlock(Block blockToAdd);
        RequestResult handleGetBlockchainRes(std::vector<Block> blocksList);
        RequestResult handleGetBlockchainReq(Message& req);

        std::vector<Block> getBlocksFromHash(std::string hash);

        // Web
        RequestResult handleAddVote(Message& req);
        std::string createVoteData(std::string vote, std::string survey_uid);

        RequestResult handleCountVotes(Message& req);
        RequestResult handlePeersList(Message& req);
        void shareBlockInTheNetwork(Block block);
        

        std::shared_ptr<Peer> _peer;
        std::shared_ptr<Blockchain> _blockchain;
};