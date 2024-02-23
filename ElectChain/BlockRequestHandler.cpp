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
        // P2P
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
        else if (reqCode == GET_BLOCKCHAIN)
        {
            std::vector<Block> blocksList = Deserializer::deserializeGetBlocks(req.buffer);
            retRes = handleGetBlockchain(blocksList);
        }
        // Web
        else if (reqCode == ADD_VOTE_CODE)
        {
            retRes = handleAddVote(req);
        }
        else if (reqCode == PEERS_LIST_SUCCEEDED_CODE)
        {
            retRes = handlePeersList(req);
        }
        else if (reqCode == COUNT_VOTES_CODE)
        {
            retRes = handleCountVotes(req);
        }
        return retRes;
    }
    else
    {
        Response res = { INVALID_REQUEST_ERROR };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
}


RequestResult BlockRequestHandler::handleAddVote(Message& req)
{
   
    try
    {
        auto blockData = Deserializer::deserializeAddVote(req.buffer);
        auto block = _blockchain->createNewBlock(createVoteData(blockData.vote, blockData.survey_uid));
        auto res = handleAddBlock(*block); 
        if (res.response.find(INVALID_REQUEST_ERROR) != std::string::npos) return res;
        else
        {
            Response res = { BlockchainUtils::_userUid + " vote added succesfully" };
            return { Serializer::serializeMessage(res, SUCCESS_ADD_VOTE) };
        }

    }
    catch (const std::exception& e)
    {
        Response res = { e.what()};
        return { Serializer::serializeMessage(res, ERROR_ADD_VOTE) };
    }
}

std::string BlockRequestHandler::createVoteData(std::string vote, std::string survey_uid)
{
    std::string voteData = "{";

    if (BlockchainUtils::isAlreadyVote(*BlockchainUtils::_bcCopy, BlockchainUtils::_userUid, survey_uid))
    {
        throw std::exception("User already vote.");
    }
    else if (BlockchainUtils::_pKeys->privateKey)
    {
        voteData += "'survey_uid':'" + survey_uid + "',";
        voteData += "'vote':'" + vote + "',";
        voteData += "'sign_vote':'" + BlockchainUtils::signMessage(vote, BlockchainUtils::_pKeys->privateKey) + "',";
        voteData += "'user_uid':'" + BlockchainUtils::_userUid + "'";
        voteData += "}";
    }
    else
    {
        throw std::exception("public key doesn't exist.");
    }

}

RequestResult BlockRequestHandler::handleCountVotes(Message& req)
{
    try
    {
        auto reqData = Deserializer::deserializeCountVotes(req.buffer); 
        CountVotesResponse votesRes(BlockchainUtils::countVotes(*BlockchainUtils::_bcCopy, reqData.survey_uid));

        return { Serializer::serializeMessage(votesRes) };
    }
    catch (const std::exception& e)
    {
        Response res = { ERROR_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
}

RequestResult BlockRequestHandler::handlePeersList(Message& req)
{
    try
    {
        auto reqData = Deserializer::deserializeGetPeers(req.buffer);

        for (auto& peer : reqData.peers)
        {
            _peer->connect(peer.peerEndpoint);
        }

        Response res = { SUCCESS_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
    catch (const std::exception& e)
    {
        Response res = { ERROR_RESPONSE };
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

    Response res = { SUCCESS_RESPONSE };
    return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
}

RequestResult BlockRequestHandler::handleAddBlock(Block blockToAdd)
{

    if (BlockchainUtils::isVoteBlock(blockToAdd) && BlockchainUtils::isValidVoteBlock(blockToAdd))
    {
        shareBlockInTheNetwork(blockToAdd);
        Response res = { SUCCESS_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
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
        Response res = { SUCCESS_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
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