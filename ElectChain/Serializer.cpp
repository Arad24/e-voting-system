#include "Serializer.h"

std::string Serializer::serializeMessageBlock(Block block)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["prevHash"] = block.getPrevHash();
    jsMsg["hash"] = block.getHash();
    jsMsg["data"] = block.getData();
    jsMsg["timestamp"] = block.getTimeStamp();
    jsMsg["nonce"] = block.getNonce();

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}

std::string Serializer::serializeMessage(Message msg)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};
    jsMsg["id"] = msg.id;
    jsMsg["buffer"] = msg.buffer;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }
    return StringUtils::vecToString(message);
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
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["public_key"] = share.public_key;
    jsMsg["user_uid"] = share.user_uid;


    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}

std::string Serializer::serializeMessage(VoteBlockData vote_block)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["user_uid"] = vote_block.user_uid;
    jsMsg["sign_vote"] = vote_block.sign_data;
    jsMsg["vote"] = vote_block.vote;
    jsMsg["survey_uid"] = vote_block.survey_uid;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
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
