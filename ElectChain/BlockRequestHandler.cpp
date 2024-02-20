#include "BlockRequestHandler.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Blockchain.h"


BlockRequestHandler::BlockRequestHandler(std::shared_ptr <Peer> peer, std::shared_ptr<Blockchain> blockchain)
{
    _peer = peer;
    _blockchain = blockchain;
}

bool BlockRequestHandler::isRequestRelevant(Message& req)
{
    return (req.id >= SHARE_KEY_CODE && req.id <= GET_BLOCKCHAIN);
}

RequestResult BlockRequestHandler::handleRequest(Message& req) 
{
    Block block;
    std::string reqCode = req.id;
    if (isRequestRelevant(req))
    {
        RequestResult retRes;
        if (reqCode == ADD_BLOCK_CODE)
        {
            block = Deserializer::deserializeMessageBlock(req.buffer);
            retRes = handleAddBlock(block);
        }
        else if (reqCode == SHARE_KEY_CODE)
        {
            block = Deserializer::deserializeMessageBlock(req.buffer);
            retRes = handleShareKey(block);
        }
        else if (reqCode == GET_PEERS_LIST) // Website
        {
            std::cout << "wait";
        }
        else if (reqCode == GET_BLOCKCHAIN)
        {
            std::vector<Block> blocksList = Deserializer::deserializeGetBlocks(req.buffer);
            retRes = handleGetBlockchain(blocksList);
        }
        else if (reqCode == LOGIN_SUCCEEDED_CODE) // Website
        {
            std::cout << "wait"; // Save in file the uid
        }
        else if (reqCode == ADD_VOTE_CODE) // Website
        {
            std::cout << "wait"; // Add vote
        }
        else if (reqCode == COUNT_VOTES_CODE) // Website
        {
            //BlockchainUtils::countVotes(std::ref(*_blockchain));
            std::cout << "wait"; // Return the votes
        }         

        return retRes;
    }
    else
    {
        Response res = { INVALID_REQUEST_ERROR };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
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
            return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
        }
    }

    // Return good response
    Response res = { INVALID_REQUEST_ERROR };
    return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
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
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
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
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
}


void BlockRequestHandler::shareBlockInTheNetwork(Block block)
{
    _blockchain->addBlock(block);
    _peer->sendBroadcastMsg(Serializer::serializeMessageBlock(block));
}