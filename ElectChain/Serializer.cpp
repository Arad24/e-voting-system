#include "Serializer.h"

std::string Serializer::serializeMessageBlock(Block block)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["prevHash"] = block.getPrevHash();
    jsMsg["hash"] = block.getHash();
    jsMsg["data"] = block.getData();
    jsMsg["timestamp"] = block.getTimeStamp();

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}

std::string Serializer::serializeMessage(const Message& msg)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};
    jsMsg["id"] = msg.id;
    jsMsg["buffer"] = msg.buffer;
    jsMsg["timestamp"] = msg.timestamp;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }
    return StringUtils::vecToString(message);
}

std::string Serializer::serializeErrorResponse(ErrorResponse errorResponse)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};
    jsMsg["message"] = errorResponse.message;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }
    return StringUtils::vecToString(message);
}

std::string Serializer::serializeSharePK(SharePK share)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["public_key"] = share.public_key;
    jsMsg["voter_id"] = share.voter_id;


    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}

std::string Serializer::serializeVoteBlock(VoteBlock vote_block)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["voter_id"] = vote_block.voter_id;
    jsMsg["sign_data"] = vote_block.sign_data;
    jsMsg["candidate"] = vote_block.candidate;
    jsMsg["survey_id"] = vote_block.survey_id;

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}
