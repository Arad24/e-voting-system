#include "BlockRequestHandler.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Blockchain.h"


BlockRequestHandler::BlockRequestHandler(std::shared_ptr <Peer> peer)
{
    _peer = peer;
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
            {
                Block block = Deserializer::deserializeMessageBlock(request.buffer);
                retRes = handleAddBlock(block);
            }
            case SHARE_KEY_CODE:
            {
                ShareKeyRequest shareKeyRequest = Deserializer::deserializeShareKey(request.buffer);
                retRes = handleShareKey(shareKeyRequest);
            }
        }

        return retRes;
    }
    else
    {
        ErrorResponse res = { "Invalid Request" };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}


RequestResult BlockRequestHandler::handleAddBlock(RequestInfo rInfo)
{
    VoteBlockRequest req = Deserializer::deserializeMessage(rInfo.buffer);
    Block blockToAdd = req.block;

    if (_peer->getBlockchain()->validateBlock(blockToAdd) && !BlockchainUtils::isAlreadyVote(BlockchainUtils::getUidFromBlock(blockToAdd))) /* signature is valid */
    {
        shareBlockInTheNetwork(blockToAdd);
    }
    else
    {
        ErrorResponse res = { "Invalid Request" };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}

RequestResult BlockRequestHandler::handleShareKey(const ShareKeyRequest& shareKeyRequest) 
{
    ShareKeyRequest req = Deserializer::deserializeMessage(rInfo.buffer);
    Block blockToAdd = req.block;

    if (_peer->getBlockchain()->validateBlock(blockToAdd) && !BlockchainUtils::isAlreadySharePK(BlockchainUtils::getUidFromBlock(blockToAdd)))
    {
        shareBlockInTheNetwork(blockToAdd);
    }
    else
    {
        ErrorResponse res = { "Invalid Request" };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}


void BlockRequestHandler::shareBlockInTheNetwork(Block block)
{
    _peer->getBlockchain()->addBlock(block);
    _peer->sendBroadcastMsg(Serializer::serializeMessageBlock(block));
}