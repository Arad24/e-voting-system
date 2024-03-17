#include "BlockRequestHandler.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Blockchain.h"


BlockRequestHandler::BlockRequestHandler(std::shared_ptr <Peer> peer, std::shared_ptr<Blockchain> blockchain)
{
    _peer = peer;
    _blockchain = blockchain;
}

bool BlockRequestHandler::isRequestRelevant(Message req)
{
    return (req.id >= ADD_BLOCK_CODE && req.id <= LOGIN_CODE) ||
           (req.id >= PEERS_LIST_SUCCEEDED_CODE && req.id <= LOGIN_SUCCEEDED_CODE) ||
           (req.id >= ADD_VOTE_CODE && req.id <= COUNT_VOTES_CODE) ||
           (req.id >= SUCCESS_ADD_VOTE && req.id <= SUCCESS_COUNT_VOTES);
}

RequestResult BlockRequestHandler::handleRequest(Message req) 
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
        else if (reqCode == GET_BLOCKCHAIN_REQ)
        {
            handleGetBlockchainReq(req);
        }
        else if (reqCode == GET_BLOCKCHAIN_RES)
        {
            std::vector<Block> blocksList = Deserializer::deserializeGetBlocksResponse(req.buffer);
            retRes = handleGetBlockchainRes(blocksList);
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
    else if (BlockchainUtils::_pKeys && BlockchainUtils::_pKeys->privateKey) 
    {
        voteData += "\"survey_uid\":\"" + survey_uid + "\",";
        voteData += "\"vote\":\"" + vote + "\",";
        std::string signature = BlockchainUtils::signMessage(vote, BlockchainUtils::_pKeys->privateKey);
        voteData += "\"sign_vote\":\"" + StringUtils::base64_encode(signature) + "\",";
        voteData += "\"user_uid\":\"" + BlockchainUtils::_userUid + "\"";
        voteData += "}";
    }
    else
    {
        throw std::exception("public key doesn't exist.");
    }

    std::cout << voteData;
    return voteData;

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
            if (_peer->getPeerAddress() != peer.toString()) _peer->connect(peer.peerEndpoint);
        }

        Response res = { SUCCESS_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        Response res = { ERROR_RESPONSE };
        return { Serializer::serializeMessage(res, DONT_SEND_CODE) };
    }
}

RequestResult BlockRequestHandler::handleGetBlockchainRes(std::vector<Block> blocksList)
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

RequestResult BlockRequestHandler::handleGetBlockchainReq(Message& req)
{
    auto reqData = Deserializer::deserializeGetBlocksRequest(req.buffer);
    

    if (reqData.last_hash == "")
    {
        GetBlocksResponse res(BlockchainUtils::_bcCopy->getBlocks());
        return { Serializer::serializeMessage(res) };
    }
    else
    {
        return { Serializer::serializeMessage(getBlocksFromHash(reqData.last_hash)) };
    }

}

std::vector<Block> BlockRequestHandler::getBlocksFromHash(std::string hash)
{
    std::vector<Block> blocks;
    bool startRead = false;

    for (auto block : BlockchainUtils::_bcCopy->getBlocks())
    {
        if (block.getHash() == hash) startRead = true;
        else if (startRead) blocks.push_back(block);
    }

    return blocks;
}

RequestResult BlockRequestHandler::handleAddBlock(Block blockToAdd)
{

    if (BlockchainUtils::isVoteBlock(blockToAdd) && BlockchainUtils::isValidVoteBlock(blockToAdd))
    {
        if (!BlockchainUtils::isAlreadySharePK(*_blockchain, BlockchainUtils::_userUid))
        {
            std::string data = BlockchainUtils::createSharedKeyData();
            if (data == "") throw std::exception("error creating share key block");
            Block* shareKeyBlock = new Block(data);
            handleShareKey(*shareKeyBlock);
        }
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