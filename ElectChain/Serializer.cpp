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
