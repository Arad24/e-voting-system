#include "Serializer.h"

std::string Serializer::serializeMessageBlock(Block block)
{
    nlohmann::json jsMsg = {
        {"prevHash", block.getPrevHash()},
        {"hash", block.getHash()},
        {"data", block.getData()},
        {"index", block.getIndex()},
        {"nonce", block.getNonce()}
    };

    std::string jsonString = jsMsg.dump();
    std::vector<unsigned char> message(jsonString.begin(), jsonString.end());
    std::string str(message.begin(), message.end());

    return str;
}

std::string Serializer::serializeMessage(Message msg)
{
    nlohmann::json jsMsg = {};
    jsMsg["id"] = msg.id;
    jsMsg["buffer"] = msg.buffer;

    std::string jsonString = jsMsg.dump();
    std::vector<unsigned char> message(jsonString.begin(), jsonString.end());
    std::string str(message.begin(), message.end());

    return str;
}

std::string Serializer::serializeMessage(Response res, std::string code)
{
    std::vector<unsigned char> message;
    
    // Push code
    for (char c : code)
    {
        message.push_back(c);
    }
    nlohmann::json jsMsg = {};
    jsMsg["message"] = res.message;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }
    return StringUtils::vecToString(message);
}

std::string Serializer::serializeMessage(SharePKData share)
{
    nlohmann::json jsMsg = {};

    jsMsg["public_key"] = share.public_key;
    jsMsg["user_uid"] = share.user_uid;


    std::string jsonString = jsMsg.dump();
    std::vector<unsigned char> message(jsonString.begin(), jsonString.end());
    std::string str(message.begin(), message.end());

    return str;
}

std::string Serializer::serializeMessage(VoteBlockData vote_block)
{
    nlohmann::json jsMsg = {};

    jsMsg["user_uid"] = vote_block.user_uid;
    jsMsg["sign_vote"] = vote_block.sign_data;
    jsMsg["vote"] = vote_block.vote;
    jsMsg["survey_uid"] = vote_block.survey_uid;

    std::string jsonString = jsMsg.dump();
    std::vector<unsigned char> message(jsonString.begin(), jsonString.end());
    std::string str(message.begin(), message.end());

    return str;
}

std::string Serializer::serializeMessage(CountVotesResponse res)
{
    nlohmann::json jsonObj;
    for (const auto& votePair : res.votes)
    {
        jsonObj[votePair.first] = votePair.second;
    }


    return SUCCESS_COUNT_VOTES + jsonObj.dump(4);
}


std::string Serializer::serializeMessage(GetBlocksResponse res)
{
    nlohmann::json jsonBlockchain;

    for (auto& block : res.blocks)
    {

        nlohmann::json jsonBlock;
        jsonBlock["blockHash"] = block.getHash();
        jsonBlock["prevHash"] = block.getPrevHash();
        jsonBlock["data"] = block.getData();
        jsonBlock["nonce"] = block.getNonce();
        jsonBlock["index"] = block.getIndex();

        jsonBlockchain.push_back(jsonBlock);
    }


    std::string retString = GET_BLOCKCHAIN_RES + std::string("{") + jsonBlockchain.dump(4) + std::string("}");
    return retString;
}