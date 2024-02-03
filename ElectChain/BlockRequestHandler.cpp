#include "BlockRequestHandler.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Blockchain.h"


BlockRequestHandler::BlockRequestHandler(std::shared_ptr <Peer> peer, std::shared_ptr<Blockchain> blockchain)
{
    _peer = peer;
    _blockchain = blockchain;
}

bool BlockRequestHandler::isRequestRelevant(RequestInfo rInfo)
{
    return (rInfo.id >= SHARE_KEY_CODE && rInfo.id <= VOTE_BLOCK_CODE);
}

RequestResult BlockRequestHandler::handleRequest(const Message& request) 
{
    if (isRequestRelevant(rInfo))
    {
        RequestResult retRes;
        switch (request.id)
        {
            case ADD_BLOCK_CODE:
                Block block = Deserializer::deserializeMessageBlock(request.buffer);
                retRes = handleAddBlock(block);
                break;

            case SHARE_KEY_CODE:
                Block block = Deserializer::deserializeMessageBlock(request.buffer);
                retRes = handleShareKey(block);
                break;

            case CREATE_BLOCK_CODE:
                break;

            case GET_BLOCKCHAIN:
                std::vector<Block> blocksList = Deserializer::deserializeGetBlocks(request.buffer);
                retRes = handleGetBlockchain(blocksList);
                break;

        }

        return retRes;
    }
    else
    {
        Response res = { INVALID_REQUEST_ERROR };
        return { Serializer::serializeMessage(res), nullptr };
    }
}


RequestResult BlockRequestHandler::handleGetBlockchain(std::vector<Block> blocksList)
{
    for (auto block : blocksList)
    {
        if ((BlockchainUtils::isVoteBlock(block) && BlockchainUtils::isValidVoteBlock(block)) ||
            ((BlockchainUtils::isShareKeyBlock(block) && BlockchainUtils::isValidShareKeyBlock(block))))
        {
            _blockchain->addBlock(block);
        }
        else
        {
            Response res = { INVALID_REQUEST_ERROR };
            return { Serializer::serializeMessage(res), nullptr };
        }
    }

    // Return good response
    return goodres;
}

RequestResult BlockRequestHandler::handleAddBlock(Block blockToAdd)
{

    if (BlockchainUtils::isVoteBlock(blockToAdd) && BlockchainUtils::isValidVoteBlock(blockToAdd))
    {
        shareBlockInTheNetwork(blockToAdd);
    }
    else
    {
        Response res = { INVALID_REQUEST_ERROR };
        return { Serializer::serializeMessage(res), nullptr };
    }
}

RequestResult BlockRequestHandler::handleShareKey(Block blockToAdd)
{

    if (BlockchainUtils::isShareKeyBlock(blockToAdd) && BlockchainUtils::isValidShareKeyBlock(blockToAdd))
    {
        shareBlockInTheNetwork(blockToAdd);
    }
    else
    {
        Response res = { INVALID_REQUEST_ERROR };
        return { Serializer::serializeMessage(res), nullptr };
    }
}


void BlockRequestHandler::shareBlockInTheNetwork(Block block)
{
    _blockchain->addBlock(block);
    _peer->sendBroadcastMsg(Serializer::serializeMessageBlock(block));
}